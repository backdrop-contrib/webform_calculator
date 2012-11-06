# Webform Calculator 
  * Provides a _formula_ webform component for _computed_ values, where you can enter a mathematical calculation based
    on the other fields. E.g., create a formula field with value ```{formkey1} * 0.1 * {formkey2}```.
  * Replaces tokens in the webform body (using webform_tokens) with [jEditable](http://www.appelsiini.net/projects/jeditable), 
    so that users can enter calculation inputs inline & get the results via AJAX.
  * You can also use the _formula_ component against a webservice, by piping other component values in as GET variables (eg, 
    http://localhost:3000/math/mult?a={a}&b={b})
  * WARNING! Webform Calculator doesn't yet support the Formula component on the standard webform output. You have to 
    edit the Body field of the webform, entering Webform Tokens where you want the inputs & formula to appear. See note at the 
    end of this README
    
# Requires
  * [Webform Tokens](http://drupal.org/project/webform_tokens)
 
# Future Plans
  * Use [Backbone](http://drupal.org/project/backbone) instead of AJAX
  * Ability to call REST for web-service-provided complex calculations (anything beyond basic math)
 
# Instructions
  1. Create a webform. Use "Number" components for anything you want used as input to calculations.
  2. To add calculations, create a component of type "Formula". You can have as many of these in a webform as you want.
    a. If you want the calculation to be basic math ({a}+{b}/{c}), set the "Calculation Method" to "Basic Math" (default).
       Then you pipe in the Field Keys of any input values wrapped in curly braces. You can use pretty much any mathematical operations.
       For example, if you have fields with Field Keys input1, input2, input3 - you set the Formula component's value to 
       ({input1}+{input2})^{input3}  
    b. If you want the calculation to be based on the results of a webservice, pipe the values in to a literal URL string. E.g.,
       set the "value" field of the Formula component to http://localhost:3000/math/mult?a={a}&b={b}
  3. Finally, use Webform Tokens to place values and inputs into the body of a webform. Edit the webform, and enter into the "Body"
     field all components you want the user to input, as well as any Formula components you want updated when they change those
     input values. E.g, edit the webform body to look like this:
     ```
     Please enter your age: [webform:webform-val-age]
     Please enter your weight: [webform:webform-val-weight]
     ____
     Your age * weight = [webform:webform-val-age_times_weight_field]
     ```
     Note that this step is currently required - Webform Calculator doesn't yet support replacing Formula components based
     on the input of fields on a standard webform's output. You have to edit the Body field and enter valid Webform Tokens.
     Stupid, I know - but my current client doesn't need it and I don't have time for stuff. FIXME.
     
# Beautytips
If you want popups when a user hovers over an editable field, download and enable [Beautytips](http://drupal.org/project/beautytips). 
Be sure to configure Beautytips to "Add beautytips js to every page" (admin/config/user-interface/beautytips), or use your own method
for adding it to specific pages.