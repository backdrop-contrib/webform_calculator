<?php

//////////////// Webform Settings Form //////////////////////

/**
 * Adds service endpoint fields to webform settings form
 */
function webform_calculator_webform_settings_form_alter(&$form, &$form_state, $form_id) {
	$form['webform_calculator_services'] = array(
		'#type' => 'fieldset',
		'#title' => 'Webform Calculator Services',
		'#collapsible' => true,
		'#description' => 'If your webform performs calculations from a web service, enter the service endpoints here. Will use stanard GET calls.'
	);
	
	// We will have many fields with the same name, so we need to be able to
	// access the form hierarchically.
	$form['#tree'] = TRUE;
	
	$services = variable_get('webform_calculator_services', array());
	if (empty($form_state['num_services'])) {
		$form_state['num_services'] = ( sizeof($services) > 0 ) ? sizeof($services) : 1;
	}
	
	// Build the number of name fieldsets indicated by $form_state['num_names']
	for ($i = 1; $i <= $form_state['num_services']; $i++) {
		$existing = !empty($services[$i]) ? $services[$i] : array('machine_name'=>'', 'endpoint' => '');
		$form['webform_calculator_services'][$i] = array(
			'#type' => 'fieldset',
			'#title' => t('Service #@num', array('@num' => $i)),
		);
	
		$form['webform_calculator_services'][$i]['machine_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Machine Name'),
			'#description' => t("Enter only lowercase alpha characters. This is used in your formula components as <em>servicename:method(component1,component2)</em>"),
			'#default_value' => $existing['machine_name'],
		);
		$form['webform_calculator_services'][$i]['endpoint'] = array(
			'#type' => 'textfield',
			'#title' => t('Service Endpoint'),
			'#description' => t("The service endpoint from which methods will be called. Must be a valid URL, like <em>http://calculations.com/rest</em>"),
			'#default_value' => $existing['endpoint'],
		);
	}
	
	// Adds "Add another name" button
	$form['webform_calculator_services']['add_service'] = array(
			'#type' => 'submit',
			'#value' => t('Add another endpoint'),
			'#submit' => array('webform_calculator_webform_settings_add_service'),
	);
	
	// If we have more than one name, this button allows removal of the
	// last name.
	if ($form_state['num_services'] > 1) {
		$form['webform_calculator_services']['remove_service'] = array(
				'#type' => 'submit',
				'#value' => t('Remove latest service'),
				'#submit' => array('webform_calculator_webform_settings_remove_service'),
				// Since we are removing a name, don't validate until later.
				'#limit_validation_errors' => array(),
		);
	}
	
	$form['#submit'][] = "webform_calculator_webform_settings_submit";
	
	return $form;
}

/**
 * Submit handler for "Add another service" button.
 *
 * $form_state['num_services'] tells the form builder function how many name
 * fieldsets to build, so here we increment it.
 *
 * All elements of $form_state are persisted, so there's no need to use a
 * particular key, like the old $form_state['storage']. We can just use
 * $form_state['num_servicess'].
 */
function webform_calculator_webform_settings_add_service($form, &$form_state) {
  // Everything in $form_state is persistent, so we'll just use
  // $form_state['add_name']
  $form_state['num_services']++;

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

function webform_calculator_webform_settings_remove_service($form, &$form_state) {
	if ($form_state['num_services'] > 1) {
		$form_state['num_services']--;
	}

	// Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
	$form_state['rebuild'] = TRUE;
}

/**
 * Submit function for webform_calculator_webform_settings().
 */
function webform_calculator_webform_settings_submit($form, &$form_state) {
	$services = array();
  for ($i = 1; $i <= $form_state['num_services']; $i++) {
  	$services[$i] = array(
  		'machine_name' => $form_state['values']['webform_calculator_services'][$i]['machine_name'],
  		'endpoint' => $form_state['values']['webform_calculator_services'][$i]['endpoint']
  	);
  }
  variable_set('webform_calculator_services', $services);
}