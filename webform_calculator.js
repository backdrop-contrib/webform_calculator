(function ($) {
  Drupal.behaviors.webform_calculator = {
    attach: function(context) {
      
      $('.webform-calculator-jeditable', context).editable(function(value, settings) {
        search_id = "#" + this.id.replace('inline', 'input');
        $input = $("#webform-calculator-form "+search_id+" :input");
        console.log($input);
        $input.val(value);
        $('#webform-calculator-form :submit').mousedown(); // see http://goo.gl/zwNdt
        return(value);
       }, { 
        indicator : 'Saving...',
        tooltip   : 'Click to edit...',
        cancel    : 'Cancel',
        submit    : 'Save',
        style     : 'display: inline; min-width: 200px;'
      });
      
    }
  };
})(jQuery);