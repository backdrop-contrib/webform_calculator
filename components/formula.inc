<?php

/**
 * @file
 * Webform Calculator formula component.
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
      'field_prefix' => '',
      'field_suffix' => '',
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      // TODO 'analysis' => FALSE,
      // TODO 'decimals' => '',
      // TODO 'point' => '.',
      // TODO 'separator' => ',',
      // TODO 'excludezero' => 0,
      'precision' => 1,
      'hidden' => FALSE,
      'error_message' => '',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_formula() {
  return array(
    'webform_calculator_formula_display' => array(
      'render element' => 'element',
    ),
    'webform_calculator_formula_form' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_calculator_formula_display($variables) {
  return theme('webform_display_number', $variables);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_formula($component, $value, $format = 'html') {
  webform_component_include('number');
  $component['extra']['decimals'] = NULL;
  $component['extra']['point'] = '.';
  $component['extra']['separator'] = ',';

  $element = _webform_display_number($component, $value, $format);
  $element['#theme'] = 'webform_calculator_formula_display';
  $element['#value'] = isset($value[0]) ? $value[0] : '';
  return $element;
}

/**
 * Theme function for formula displayed on webform view.
 */
function theme_webform_calculator_formula_form($variables) {
  $component = $variables['component'];
  return '<span id="formula-component-' . $component['form_key'] . '">' . $component['value'] . '</span>';
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_formula($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#printed' => $component['extra']['hidden'],
    '#type' => 'markup',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : ($filter ? webform_filter_xss($component['extra']['field_prefix']) : $component['extra']['field_prefix']),
    '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : ($filter ? webform_filter_xss($component['extra']['field_suffix']) : $component['extra']['field_suffix']),
    '#description' => $filter
      ? webform_filter_descriptions($component['extra']['description'])
      : $component['extra']['description'],
    '#markup' => theme('webform_calculator_formula_form', array('component' => $component)),
    '#translatable' => array('title', 'description'),
    '#theme_wrappers' => array('webform_element'),
  );
  $element['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array(
      'webformCalculator' => array($component['cid'] => $component),
    ),
  );
  // Adding module JS file.
  $element['#attached']['js'][] = array(
    'type' => 'file',
    'data' => drupal_get_path('module', 'webform_calculator') . '/webform_calculator.js',
  );

  return $element;
}

/**
 * Reusable constant.
 */
function _webform_calculator_allowed_component_types() {
  return array('number' => 1, 'formula' => 1, 'select' => 1);
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_formula($component) {
  $allowed_component_types = _webform_calculator_allowed_component_types();
  $webform_node = node_load($component['nid']);

  // Create list of available tokens for formula description.
  $available_components = array();
  foreach ($webform_node->webform['components'] as $node_component) {
    if (isset($allowed_component_types[$node_component['type']])) {
      $available_components[] = '{' . $node_component['form_key'] . '} — ' . $node_component['name'];
    }
  }
  $replacements = theme('item_list', array('title' => t('Available replacement tokens'), 'items' => $available_components));
  $fieldset = array(
    '#title' => t('Token values'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#children' => '<div>' . $replacements . '</div>',
    '#attributes' => array('class' => array('collapsible', 'collapsed')),
  );
  $fieldset = theme('fieldset', array('element' => $fieldset));

  $form = array();

  // Create description for formula value.
  $element_description = t('Enter the calculation formula with components in brackets.');
  $allowed_operators = '+, -, *, /, (, ), %, ?, :, <, >, <=, >=, ==, &&, ||';
  $allowed_operators_message = t('Allowed operators: %operators', array('%operators' => $allowed_operators));
  $example_formula = '{component1} + {component2} / {component3}';
  $available_tokens_message = t('Example: %example', array('%example' => $example_formula));

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Formula value'),
    '#maxlength' => 1028,
    '#default_value' => $component['value'],
    '#description' => "$element_description<br/>$allowed_operators_message<br/>$available_tokens_message<br/>$fieldset",
    '#weight' => -3,
    '#element_validate' => array('_webform_edit_formula_validate'),
  );

  $form['display']['field_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix text placed to the left of the field'),
    '#default_value' => $component['extra']['field_prefix'],
    '#description' => t('Examples: $, #, -.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#parents' => array('extra', 'field_prefix'),
  );

  $form['display']['field_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Postfix text placed to the right of the field'),
    '#default_value' => $component['extra']['field_suffix'],
    '#description' => t('Examples: lb, kg, %.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#parents' => array('extra', 'field_suffix'),
  );

  $form['display']['error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Error message'),
    '#default_value' => $component['extra']['error_message'],
    '#description' => t('This message will be displayed when values of components inside formula will be empty or incorrect. By default user will see <strong>Enter correct value for %fields to see result</strong> message. Leave this field empty to use default message.'),
    '#parents' => array('extra', 'error_message'),
  );

  $form['display']['precision'] = array(
    '#type' => 'select',
    '#title' => t('Precision'),
    '#default_value' => $component['extra']['precision'],
    '#description' => t('Number of significant digits (after the decimal point).'),
    '#options' => range(0, 10),
    '#parents' => array('extra', 'precision'),
  );

  $form['display']['hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hidden'),
    '#default_value' => $component['extra']['hidden'],
    '#weight' => 11,
    '#description' => t('Hide result of this formula. This will hide component on form but will save result to database.'),
    '#parents' => array('extra', 'hidden'),
  );

  return $form;
}

/**
 * Element validate handler; Set the precision value.
 */
function _webform_edit_formula_validate($element, &$element_state) {
  $original_formula = $formula = $element['#value'];
  $formula = $element['#value'];

  // Replace elements in {}.
  $formula = preg_replace(WEBFORM_CALCULATOR_REGEX, 0, $formula);

  try {
    webform_calculator_validate_php($formula);
  }
  catch (Exception $e) {
    form_error($element, t('The formula syntax is invalid. !error', array('!error' => $e->getMessage())));
  }

  $components_from_formula = webform_calculator_get_components_from_formula($original_formula);
  $webform_node = node_load($element_state['complete form']['nid']['#value']);

  // Get webform components keyed by form key.
  $components_by_key = array();
  foreach ($webform_node->webform['components'] as $component) {
    $components_by_key[$component['form_key']] = $component;
  }

  $allowed_component_types = _webform_calculator_allowed_component_types();
  foreach ($components_from_formula as $component_key) {
    if (!isset($components_by_key[$component_key])) {
      form_error($element, t('Webform component %component does not exist.', array('%component' => $component_key)));
    }
    elseif (!isset($allowed_component_types[$components_by_key[$component_key]['type']])) {
      form_error($element, t('Entered component %component is not numeric type.', array('%component' => $component_key)));
    }
  }
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_formula($component, $value) {

}

/**
 * Implements _webform_table_component().
 */
function _webform_table_formula($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_formula($component, $export_options) {
  return array(
    0 => '',
    1 => '',
    2 => $export_options['header_keys'] ? $component['form_key'] : $component['name'],
  );
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_formula($component, $export_options, $value) {
  return isset($value[0]) ? $value[0] : '';
}
