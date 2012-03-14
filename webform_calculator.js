(function ($) {
  Drupal.behaviors.webform_calculator = {
    attach: function(context) {
      
      $('.sharpe-jeditable', context).editable(function(value, settings) {
        search_id = "#" + this.id.replace('inline', 'input');
        $input = $("#sharpe-calc-form "+search_id+" :input");
        console.log($input);
        $input.val(value);
        $('#sharpe-calc-form :submit').mousedown(); // see http://goo.gl/zwNdt
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