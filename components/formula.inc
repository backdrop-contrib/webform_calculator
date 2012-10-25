<?php

/**
 * @file
 * Webform module markup component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_formula() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'private' => FALSE, //TODO ?
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_formula($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
  	'#size' => 256,
  	'#maxlength' => 1028,
    '#default_value' => $component['value'],
    '#description' => t('Enter the calculation with components in brackets.<br/>
    		Standard math example: <em>({component1}+{component2})/{component3}</em><br/>
    		Web service example: <em>http://calculation-service.com/mathMethod?val1={component1}&val2={component2}</em>
    		'),
    '#weight' => -1,
    '#element_validate' => array('_webform_edit_formula_validate'),
  );
  $form['extra']['method'] = array(
  		'#type' => 'select',
  		'#title' => 'Calculation Method',
  		'#options' => array(
  				'math' => 'Basic Math',
  				'service' => 'Web Service'
  		),
  		'#default_value' => !empty($component['extra']['method']) ? $component['extra']['method'] : 'math',
  		'#description' =>t('Formula allows you to compute the values of other components via standard math operations or via web services.'),
  );
  $form['extra']['precision'] = array(
    '#type' => 'textfield',
    '#title' => t('Precision'),
    '#size' => 3,
    '#default_value' => !empty($component['extra']['precision']) ? $component['extra']['precision'] : NULL,
    '#description' => t('Number of significant digits (after the decmial point).'),
  );
  $form['extra']['description'] = array(); // No description for formula.
  $form['display'] = array('#type' => 'markup'); // Hide the display options.

  return $form;
}

/**
 * Element validate handler; Set the precision value.
 */
function _webform_edit_formula_validate($form, &$form_state) {
  if (is_array($form_state['values']['value'])) {
    $form_state['values']['extra']['precision'] = $form_state['values']['value']['precision'];
    $form_state['values']['value'] = $form_state['values']['value']['value'];
  }
}

/**
 * Implements _webform_render_component().
 * This function is loaded too late in the game, the REAL render functionality
 * is in webform_calculator.module hook_node_load()
 */
function _webform_render_formula($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? NULL : $component['name'],
    '#weight' => $component['weight'],
    '#markup' => $filter ? _webform_filter_values(check_plain($component['value']), NULL, NULL, NULL, FALSE) : $component['value'],
    '#theme_wrappers' => array('webform_element'),
    '#webform_component' => $component,
  );

  // TODO: Remove when #markup becomes available in D7.
  $element['#value'] = $element['#markup'];

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_formula($component, $value) {
  return array();
}
