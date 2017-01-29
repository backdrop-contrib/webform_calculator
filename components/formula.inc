<?php

/**
 * @file
 * Webform Calculator formula component.
 */

// Needed for conditional rules and component display
webform_component_include('number');

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
      'width' => '',
      'field_prefix' => '',
      'field_suffix' => '',
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      // TODO 'analysis' => FALSE,
      'point' => '.',
      'separator' => ',',
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
  $component['extra']['decimals'] = NULL;

  $element = _webform_display_number($component, $value, $format);
  $element['#theme'] = 'webform_calculator_formula_display';
  $element['#value'] = isset($value[0]) ? _webform_number_format($component, $value[0]) : '';
  return $element;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_formula($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#theme_wrappers' => array('webform_element'),
    '#weight' => $component['weight'],
  );
  if (!$component['extra']['hidden']) {
    $element += array(
      '#type' => 'textfield',
      '#attributes' => array('readonly' => TRUE),
      '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : ($filter ? webform_filter_xss($component['extra']['field_prefix']) : $component['extra']['field_prefix']),
      '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : ($filter ? webform_filter_xss($component['extra']['field_suffix']) : $component['extra']['field_suffix']),
      '#description' => $filter
        ? webform_filter_descriptions($component['extra']['description'])
        : $component['extra']['description'],
      '#translatable' => array('title', 'description'),
    );

    // Change the 'width' option to the correct 'size' option.
    if ($component['extra']['width'] > 0) {
      $element['#size'] = $component['extra']['width'];
    }
  }
  else {
    $element['#type'] = 'hidden';
  }
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
  $element['#attached']['js'][] = array(
    'type' => 'file',
    'data' => drupal_get_path('module', 'webform_calculator') . '/parser.js',
  );

  return $element;
}

/**
 * Reusable constant.
 */
function _webform_calculator_allowed_component_types() {
  return array('number' => 1, 'formula' => 1, 'select' => 1, 'grid' => 1);
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_formula($component) {
  module_load_include('php', 'webform_calculator', 'matheval.class');
  $available_components = _webform_calculator_get_tokens($component);

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
  $parser = new WebformEvalMath();
  $example_tokens = array_keys($available_components) + array('a', 'b', 'c');
  $description = array('<em>' . t('Enter the calculation formula with components in brackets.') . '</em>');
  $allowed_operators = array('+', '-', '*', '/', '^', '_');
  $description[] = '<strong>' . t('Operators:') . '</strong> ' . implode(', ', $allowed_operators);
  $description[] = '<strong>' . t('Constants:') . '</strong> ' . implode(', ', $parser->vb);
  $description[] = '<strong>' . t('Functions:') . '</strong> ' . implode(', ', $parser->fb);
  $description[] = '<strong>' . t('Example:') . "</strong> ({{$example_tokens[0]}} + {{$example_tokens[1]}}) ^ {{$example_tokens[2]}}";


  $form['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Formula value'),
    '#required' => TRUE,
    '#rows' => 3,
    '#default_value' => $component['value'],
    '#description' => implode('<br />', $description) . "<br/>$fieldset",
    '#weight' => -3,
    '#attributes' => array('placeholder' => "{{$example_tokens[0]}} + {{$example_tokens[1]}}"),
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

  $form['display']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the textfield.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#parents' => array('extra', 'width'),
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

  $form['display']['separator'] = array(
    '#type' => 'select',
    '#title' => t('Thousands separator'),
    '#default_value' => $component['extra']['separator'],
    '#options' => array(',' => t('Comma (,)'), '.' => t('Period (.)'), ' ' => t('Space ( )'), '' => t('None')),
    '#parents' => array('extra', 'separator'),
  );

  $form['display']['point'] = array(
    '#type' => 'select',
    '#title' => t('Decimal point'),
    '#default_value' => $component['extra']['point'],
    '#options' => array(',' => t('Comma (,)'), '.' => t('Period (.)')),
    '#parents' => array('extra', 'point'),
    '#element_validate' => array('_webform_edit_separator_point_validate'),
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
 * Element validate handler; Check the separator and decimal point value.
 */
function _webform_edit_separator_point_validate($element, &$element_state) {
  if (!empty($element_state['values']['extra']['separator'])) {
    if ($element_state['values']['extra']['point'] === $element_state['values']['extra']['separator']) {
      form_error($element, t('The decimal point and the thousands separator could not be the same.'));
    }
  }
}

/**
 * Element validate handler; Set the precision value.
 */
function _webform_edit_formula_validate($element, &$element_state) {

  $original_formula = $formula = $element['#value'];
  $formula = $element['#value'];

  // Verify that the value is not longer than 1028 characters.
  if (drupal_strlen($formula) > 1028) {
    form_error($element, t('!name cannot be longer than %max characters but is currently %length characters long.', 
      array('!name' => $element['#title'], '%max' => 1028, '%length' => drupal_strlen($formula))));
  }

  // Replace elements in {}.
  $formula = preg_replace(WEBFORM_CALCULATOR_REGEX, '(1)', $formula);

  try {
    webform_calculator_eval($formula);
  }
  catch (Exception $e) {
    // Div by zero isn't a syntax error, might have just been caused by our arbitrary variable substitution
    if ($e->getMessage() != 'division by zero') {
      form_error($element, t('The formula syntax is invalid. !error', array('!error' => $e->getMessage())));
      return;
    }
  }

  // In form_buider_webform context it's tricky to get the nid & cid, normally it's in the values
  $component = isset($element_state['complete form']['#_edit_element']) ? $element_state['complete form']['#_edit_element']['#webform_component'] : $element_state['values'];

  $allowed_tokens = _webform_calculator_get_tokens($component);
  $components_from_formula = webform_calculator_get_components_from_formula($original_formula);

  foreach ($components_from_formula as $component_key) {
    if (!isset($allowed_tokens[$component_key])) {
      form_error($element, t('Invalid token %key.', array('%key' => '{' . $component_key . '}')));
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

/**
 * Create list of available tokens for formula description.
 *
 * @param $component
 * @return array
 */
function _webform_calculator_get_tokens($component) {
  $allowed_component_types = _webform_calculator_allowed_component_types();
  $webform_node = node_load($component['nid']);

  if (module_exists('form_builder_webform') && class_exists('FormBuilderLoader') && arg(2) == 'form-builder' && arg(4) == 'webform') {
    $cachedForm = FormBuilderLoader::instance()->fromCache('webform', $component['nid']);
    $all_components = $cachedForm->getComponents($webform_node);
  }
  else {
    $all_components = $webform_node->webform['components'];
  }

  $available_components = array();
  $cid = isset($component['cid']) ? $component['cid'] : NULL;
  foreach ($all_components as $node_component) {
    if (isset($allowed_component_types[$node_component['type']]) && (empty($node_component['cid']) || $node_component['cid'] != $cid)) {
      $available_components[$node_component['form_key']] = '{' . $node_component['form_key'] . '} — ' . $node_component['name'];
    }
  }
  return $available_components;
}


/**
 * Implements _webform_form_builder_map_component().
 */
function _webform_form_builder_map_formula() {
  return array(
    'form_builder_type' => 'formula',
    'properties' => array(
      'value' => array(
        'form_parents' => array('value'),
        'storage_parents' => array('value'),
      ),
      'hidden' => array(
        'form_parents' => array('display', 'hidden'),
        'storage_parents' => array('extra', 'hidden'),
      ),
      'precision' => array(
        'form_parents' => array('display', 'precision'),
        'storage_parents' => array('extra', 'precision'),
      ),
      'point' => array(
        'form_parents' => array('display', 'point'),
        'storage_parents' => array('extra', 'point'),
      ),
      'separator' => array(
        'form_parents' => array('display', 'separator'),
        'storage_parents' => array('extra', 'separator'),
      ),
      'error_message' => array(
        'form_parents' => array('display', 'error_message'),
        'storage_parents' => array('extra', 'error_message'),
      ),
    ),
  );
}
