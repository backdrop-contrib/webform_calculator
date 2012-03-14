<?php
/**
 * @file
 * webform_calculator.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function webform_calculator_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_example_calculation';
  $strongarm->value = 0;
  $export['comment_anonymous_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_example_calculation';
  $strongarm->value = 1;
  $export['comment_default_mode_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_example_calculation';
  $strongarm->value = '50';
  $export['comment_default_per_page_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_example_calculation';
  $strongarm->value = '0';
  $export['comment_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_example_calculation';
  $strongarm->value = 1;
  $export['comment_form_location_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_example_calculation';
  $strongarm->value = '1';
  $export['comment_preview_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_example_calculation';
  $strongarm->value = 1;
  $export['comment_subject_field_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_example_calculation';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_example_calculation';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_example_calculation';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_example_calculation';
  $strongarm->value = '1';
  $export['node_preview_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_example_calculation';
  $strongarm->value = 0;
  $export['node_submitted_example_calculation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'webform_node_types';
  $strongarm->value = array(
    0 => 'example_calculation',
    1 => 'webform',
  );
  $export['webform_node_types'] = $strongarm;

  return $export;
}
