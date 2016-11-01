(function($) {

  Drupal.behaviors.webformCalculator = {
    attach: function(context, settings) {
      Drupal.webformCalculator.evaluateAllFormulas();

      $.each(settings.webformCalculator, function(index, component) {
        var elements = Drupal.webformCalculator.getComponentsKeys(component);

        $(elements).each(function(i, componentKey) {
          var handler = function () {
            Drupal.webformCalculator.evaluateFormula(component);
          };
          var selector = ''
                  + 'input[name$="[' + componentKey + ']"]' // Number, Single select
                  + ', '
                  + 'select[name$="[' + componentKey + ']"]' // Number, Single select
                  + ', '
                  + 'select[name$="[' + componentKey + '][]"]'// Multiple select
                  + ', '
                  + 'input:radio[name*="[' + componentKey + ']"]'// Radios, grid
                  + ', '
                  + '#edit-submitted-' + componentKey + ' input:checkbox' // Checkboxes
                  ;

          $(selector, context)
            .bind('change', handler) // Something changed
            .bind('keyup', handler) // Even before we leave input element
            .bind('mouseup', handler) // Also care for paste context menu
          ;
        });
      });
    }
  };

  Drupal.webformCalculator = {};

  // Get keys of components that were used inside formula.
  Drupal.webformCalculator.getComponentsKeys = function(formulaComponent) {
    return formulaComponent.value.match(/[^{}]+(?=\})/g);
  };

  // Replace tokens from formula with numeric values from components.
  Drupal.webformCalculator.evaluateFormula = function(formulaComponent) {
    var formulaReplaced = formulaComponent.value;
    var elements = Drupal.webformCalculator.getComponentsKeys(formulaComponent);

    var invalidFields = [];
    $(elements).each(function(index, componentKey) {
      var componentValue;
      // Number
      componentValue = componentValue ||
        $('input:text[name$="[' + componentKey + ']"]').val();
      // Single select
      componentValue = componentValue ||
        $('select[name$="[' + componentKey + ']"]').val();
      // Multiple select (provides array if exists)
      componentValue = componentValue ||
        $('select[name$="[' + componentKey + '][]"]').val();
      // Radios and checkboxes (provides array but only if exists)
      var checkables = $('#edit-submitted-' + componentKey + ' input:checkbox'
      + ', '
      + 'input:radio[name*="[' + componentKey + ']"]'
      );
      if (checkables.length > 0) {
        componentValue = componentValue ||
        checkables.map(
            function() {return this.checked ? this.value : undefined;}
        ).get();
      }
      // Care for array
      if (componentValue && componentValue instanceof Array) {
        // Convert to number if possible
        componentValue = componentValue.map(function (a) {return parseFloat(a);});
        // Summarize
        componentValue = componentValue.length ? componentValue.reduce(function (a, b) {return isNaN(a) || isNaN(b) ? NaN : a + b;}) : '';
      }

      if (isNaN(componentValue) || componentValue === '') {
        var label =  $('label[for$=-' + componentKey.replace(/_/g, '-') + ']').text().trim();
        if (label == '') {
          label = componentKey;
        }
        invalidFields.push(label);
      }
      else {
        formulaReplaced = formulaReplaced.replace('{' + componentKey + '}', '(' + componentValue + ')');
      }
    });

    var formulaComponentElement = $('input[name$="[' + formulaComponent.form_key + ']"]');

    if (invalidFields.length > 0) {
      invalidFields = Drupal.webformCalculator.unique(invalidFields);
      // Set message.
      var message = formulaComponent.extra.error_message || Drupal.t('Enter correct value for !fields to see result.', {'!fields': invalidFields.join(', ')});
      formulaComponentElement.attr('placeholder', message).val('').change();
    }
    else {
      // Set result.
      var formulaResult = eval(formulaReplaced);
      formulaResult = Drupal.webformCalculator.round(formulaResult, formulaComponent.extra.precision);
      formulaComponentElement.removeAttr('placeholder').val(formulaResult).change();
    }
  };

  // Evaluate all formulas.
  Drupal.webformCalculator.evaluateAllFormulas = function() {
    $.each(Drupal.settings.webformCalculator, function(i, component) {
      Drupal.webformCalculator.evaluateFormula(component);
    });
  };

  // Get unique elements from array.
  Drupal.webformCalculator.unique = function(array) {
     var u = {}, a = [];
     for (var i = 0, l = array.length; i < l; ++i) {
      if (u.hasOwnProperty(array[i])) {
       continue;
     }
     a.push(array[i]);
     u[array[i]] = 1;
    }
    return a;
  };

  // Round result of calculation.
  Drupal.webformCalculator.round = function(number, places) {
    var multiplier = Math.pow(10, places);
    return (Math.round(number * multiplier) / multiplier);
  }

})(jQuery);
