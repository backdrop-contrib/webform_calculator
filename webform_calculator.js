(function($, Drupal) {

  Drupal.behaviors.webform_calculator = {
    attach: function(context, settings) {
      for (var index in settings.webform_calculator) {
        var component = settings.webform_calculator[index];
        var elements = Drupal.webform_calculator.getComponentsKeys(component);

        Drupal.webform_calculator.replaceTokens(component);

        $(elements).each(function(index, componentKey) {
          $('input[name$="[' + componentKey + ']"]', context).unbind('keyup').bind('keyup', {comp: component}, function(event) {
              Drupal.webform_calculator.replaceTokens(event.data.comp);
          });
        });
      }
    }
  };

  Drupal.webform_calculator = {};

  Drupal.webform_calculator.getComponentsKeys = function(formulaComponent) {
    return formulaComponent.value.match(/[^{}]+(?=\})/g);
  };

  Drupal.webform_calculator.replaceTokens = function(formulaComponent) {
    // console.log(formulaComponent);
    var formulaReplaced = formulaComponent.value;
    var elements = Drupal.webform_calculator.getComponentsKeys(formulaComponent);

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
      invalidFields = Drupal.webform_calculator.unique(invalidFields);
      var message = Drupal.t('Enter correct value for %fields to see result.', { '%fields': invalidFields.join(', ') });
      $('#formula-component-' + formulaComponent.form_key).html(message);
    }
    else {
      var formulaResult = eval(formulaReplaced);
      formulaResult = Drupal.webform_calculator.round(formulaResult, formulaComponent.extra.precision);
      $('#formula-component-' + formulaComponent.form_key).text(formulaResult);
    }
  };

  Drupal.webform_calculator.unique = function(array) {
     var u = {}, a = [];
     for(var i = 0, l = array.length; i < l; ++i) {
      if (u.hasOwnProperty(array[i])) {
       continue;
     }
     a.push(array[i]);
     u[array[i]] = 1;
    }
    return a;
  };

  Drupal.webform_calculator.round = function(number, places) {
    var multiplier = Math.pow(10, places);
    return (Math.round(number * multiplier) / multiplier);
  }

})(jQuery, Drupal);
