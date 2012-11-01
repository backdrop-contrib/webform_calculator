This module
 * Provides a _formula_ webform component for _computed_ values, where you can enter a mathematical calculation based
   on the other fields. E.g., create a formula field with value ```formkey1 * 0.1 * formkey2``.
 * Replaces tokens in the webform body (using webform_tokens) with [jEditable](http://www.appelsiini.net/projects/jeditable), 
   so that users can enter calculation inputs inline & get the results via AJAX.
   
Requires
 * [Webform Tokens](http://drupal.org/project/webform_tokens)
 
Future Plans
 * Use [Backbone](http://drupal.org/project/backbone) instead of AJAX
 * Ability to call REST for web-service-provided complex calculations (anything beyond basic math) 
