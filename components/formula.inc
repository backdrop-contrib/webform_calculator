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
      'precision' => 1,
      'hidden' => FALSE,
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_formula($component) {
  $webform_node = node_load($component['nid']);
  $available_components = array();
  foreach ($webform_node->webform['components'] as $node_component) {
    if ($node_component['type'] == 'number') {
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
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Formula value'),
    '#maxlength' => 1028,
    '#default_value' => $component['value'],
    '#description' => t('Enter the calculation with components in brackets.<br/>Example: <em>({component1} + {component2}) / {component3}</em>') . $fieldset,
    '#weight' => -3,
    '#element_validate' => array('_webform_edit_formula_validate'),
  );
  $form['extra']['precision'] = array(
    '#type' => 'select',
    '#title' => t('Precision'),
    '#default_value' => $component['extra']['precision'],
    '#description' => t('Number of significant digits (after the decimal point).'),
    '#options' => range(0, 10),
    '#weight' => -2,
  );
  $form['extra']['hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hidden'),
    '#default_value' => $component['extra']['hidden'],
    '#description' => t('Hide result of this formula. This will hide component but will save result to database.'),
    '#weight' => -1,
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
    form_error($element, t('The formula syntax is invalid. @error', array('@error' => $e->getMessage())));
  }

  $components_from_formula = webform_calculator_get_components_from_formula($original_formula);
  $webform_node = node_load($element_state['complete form']['nid']['#value']);

  // Get webform components keyed by form key.
  $components_by_key = array();
  foreach ($webform_node->webform['components'] as $component) {
    $components_by_key[$component['form_key']] = $component;
  }

  foreach ($components_from_formula as $component_key) {
    if (!isset($components_by_key[$component_key])) {
      form_error($element, t('Webform component %component does not exist.', array('%component' => $component_key)));
    }
    elseif ($components_by_key[$component_key]['type'] != 'number') {
      form_error($element, t('Entered component %component is not numeric type.', array('%component' => $component_key)));
    }
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_formula($component, $value = NULL, $filter = TRUE) {
  if ($component['extra']['hidden']) {
    return;
  }

  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#markup' => '<span id="formula-component-' . $component['form_key'] . '">' . $component['value'] . '</span>',
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#weight' => $component['weight'],
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
 * Implements _webform_display_component().
 */
function _webform_display_formula($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_calculator_formula',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title'),
  );
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
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_formula($component, $export_options, $value) {
  return isset($value[0]) ? $value[0] : '';
}
