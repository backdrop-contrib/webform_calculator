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
    '#type' => 'text_format',
    '#title' => t('Value'),
    '#default_value' => $component['value'],
    '#description' => t('Formula allows you compute the values of other components.') . theme('webform_token_help'),
    '#weight' => -1,
    '#format' => $component['extra']['format'],
    '#element_validate' => array('_webform_edit_formula_validate'),
  );

  $form['extra']['description'] = array(); // No description for formula.
  $form['display'] = array('#type' => 'markup'); // Hide the display options.

  return $form;
}

/**
 * Element validate handler; Set the text format value.
 */
function _webform_edit_formula_validate($form, &$form_state) {
  if (is_array($form_state['values']['value'])) {
    $form_state['values']['extra']['format'] = $form_state['values']['value']['format'];
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
    '#markup' => $filter ? _webform_filter_values(check_markup($component['value'], $component['extra']['format'], '', TRUE), NULL, NULL, NULL, FALSE) : $component['value'],
    '#format' => $component['extra']['format'],
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
function _webform_display_formula($component, $value, $format = 'html') {
  return array();
}