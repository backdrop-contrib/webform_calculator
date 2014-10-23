(function($) {

  Drupal.behaviors.webformCalculator = {
    attach: function(context, settings) {
      Drupal.webformCalculator.evaluateAllFormulas();

      for (var index in settings.webformCalculator) {
        var component = settings.webformCalculator[index];
        var elements = Drupal.webformCalculator.getComponentsKeys(component);

        $(elements).each(function(index, componentKey) {
          $('input[name$="[' + componentKey + ']"]', context).unbind('keyup').bind('keyup', function(event) {
            Drupal.webformCalculator.evaluateAllFormulas();
          });
        });
      }
    }
  };

  Drupal.webformCalculator = {};

  // Get keys of components that were used inside formula.
  Drupal.webformCalculator.getComponentsKeys = function(formulaComponent) {
    return formulaComponent.value.match(/[^{}]+(?=\})/g);
  };

  // Replace tokens from formula with numeric values from components.
  Drupal.webformCalculator.evaluateFormula = function(formulaComponent) {
    console.log(formulaComponent);
    var formulaReplaced = formulaComponent.value;
    var elements = Drupal.webformCalculator.getComponentsKeys(formulaComponent);

    var invalidFields = [];
    $(elements).each(function(index, componentKey) {
      var componentValue = $('input[name$="[' + componentKey + ']"]').val();
      if (isNaN(componentValue) || componentValue == '') {
        var label =  $('label[for$=-' + componentKey.replace('_', '-') + ']').text().trim();
        if (label == '') {
          label = componentKey;
        }
        invalidFields.push(label);
      }
      else {
        formulaReplaced = formulaReplaced.replace('{' + componentKey + '}', '(' + componentValue + ')');
      }
    });

    if (invalidFields.length > 0) {
      invalidFields = Drupal.webformCalculator.unique(invalidFields);
      var message = Drupal.t('Enter correct value for %fields to see result.', {'%fields': invalidFields.join(', ')});
      $('#formula-component-' + formulaComponent.form_key).html(message);
    }
    else {
      var formulaResult = eval(formulaReplaced);
      formulaResult = Drupal.webformCalculator.round(formulaResult, formulaComponent.extra.precision);
      $('#formula-component-' + formulaComponent.form_key).text(formulaResult);
    }
  };

  // Evaluate all formulas.
  Drupal.webformCalculator.evaluateAllFormulas = function() {
    for (var index in Drupal.settings.webformCalculator) {
      var component = Drupal.settings.webformCalculator[index];
      Drupal.webformCalculator.evaluateFormula(component);
    }
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
