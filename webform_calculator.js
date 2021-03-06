(function($) {

  // Parse float according to display preferences
  function pf(num, separator, point) {
    if (typeof num === 'string') {
      num = num.split(separator).join('');
      if (point && point !== '.') {
        num = num.replace(point, '.');
      }
      num = num.replace(/ /g, '');
    }
    num = parseFloat(num);
    return isNaN(num) ? null : num;
  }

  // Round and format number according to display preferences
  function formatNumber(number, places, sep, point) {
    var multiplier = Math.pow(10, places),
      result = (Math.round(number * multiplier) / multiplier).toString();
    if (point && point !== '.') {
      result = result.replace('.', point);
    }
    if (sep) {
      result = result.replace(/\B(?=(\d{3})+(?!\d))/g, sep);
    }
    return result;
  }

  Backdrop.behaviors.webformCalculator = {
    attach: function(context, settings) {
      Backdrop.webformCalculator.evaluateAllFormulas();

      $.each(settings.webformCalculator, function(index, component) {
        var elements = Backdrop.webformCalculator.getComponentsKeys(component);

        $(elements).each(function(i, componentKey) {
          var handler = function () {
            Backdrop.webformCalculator.evaluateFormula(component);
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

  Backdrop.webformCalculator = {};

  // Get keys of components that were used inside formula.
  Backdrop.webformCalculator.getComponentsKeys = function(formulaComponent) {
    return formulaComponent.value.match(/[^{}]+(?=\})/g);
  };

  // Replace tokens from formula with numeric values from components.
  Backdrop.webformCalculator.evaluateFormula = function(formulaComponent) {
    var formulaReplaced = formulaComponent.value;
    var elements = Backdrop.webformCalculator.getComponentsKeys(formulaComponent);

    var invalidFields = [];
    $(elements).each(function(index, componentKey) {
      // Number
      var componentValue = pf($('input:text[name$="[' + componentKey + ']"]').val(), formulaComponent.extra.separator, formulaComponent.extra.point);
      // Single select
      if (componentValue === null) {
        componentValue = pf($('select[name$="[' + componentKey + ']"]').val(), formulaComponent.extra.separator, formulaComponent.extra.point);
      }
      // Multiple select (provides array if exists)
      if (componentValue === null) {
        componentValue = $('select[name$="[' + componentKey + '][]"]').val() || null;
      }
      // Radios and checkboxes (provides array but only if exists)
      if (componentValue === null) {
        var checkables = $('#edit-submitted-' + componentKey + ' input:checkbox, input:radio[name*="[' + componentKey + ']"]');
        if (checkables.length > 0) {
          componentValue = componentValue ||
            checkables.map(
              function () {
                return this.checked ? this.value : null;
              }
            ).get();
        }
      }

      // Get value from previous page submission.
      if (componentValue === null && Backdrop.settings.webformCalculatorData !== undefined && componentKey in Backdrop.settings.webformCalculatorData) {
        componentValue = Backdrop.settings.webformCalculatorData[componentKey]; 
      }

      // Care for array
      if (componentValue instanceof Array) {
        // Convert to number if possible
        componentValue = componentValue.map(function(val) {
          return pf(val, formulaComponent.extra.separator, formulaComponent.extra.point);
        });
        // Summarize
        componentValue = componentValue.length ? componentValue.reduce(function (a, b) {return a === null || b === null ? 0 : a + b;}, 0) : null;
      }

      if (componentValue === null) {
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
      invalidFields = Backdrop.webformCalculator.unique(invalidFields);
      // Set message.
      var message = formulaComponent.extra.error_message || Backdrop.t('Enter correct value for !fields to see result.', {'!fields': invalidFields.join(', ')});
      formulaComponentElement.attr('placeholder', message).val('').change();
    }
    else {
      // Set result.
      var parser = new Backdrop.WebformCalculatorParser();
      var formulaResult = parser.evaluate(formulaReplaced, {pi: Math.PI, e: Math.E});
	    formulaResult = formatNumber(formulaResult, formulaComponent.extra.precision, formulaComponent.extra.separator, formulaComponent.extra.point);
      formulaComponentElement.removeAttr('placeholder').val(formulaResult).change();
    }
  };

  // Evaluate all formulas.
  Backdrop.webformCalculator.evaluateAllFormulas = function() {
    $.each(Backdrop.settings.webformCalculator, function(i, component) {
      Backdrop.webformCalculator.evaluateFormula(component);
    });
  };

  // Get unique elements from array.
  Backdrop.webformCalculator.unique = function(array) {
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

})(jQuery);
