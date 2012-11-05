<?php

/**
 * Implements hook_variable_info().
 */
function pane_variable_info($options) {
  $variables = array();

  $variables['pane_panes'] = array(
    'type' => 'text',
    'title' => t('Define available panes here'),
    'description' => t('Format is "variable|title", one per line. Variable name will have "pane_" prepended to it.'),
    'group' => 'pane_settings',
  );

  foreach (pane_get_panes() as $id => $title) {
    $variables[$id .'_[pane_part]'] = array(
      'type' => 'pane_content',
      'group' => 'pane',
      'localize' => TRUE,
      'title' => $title,
    );
  }
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function pane_variable_group_info() {
  $groups = array();

  $groups['pane_settings'] = array(
    'title' => t('Pane settings'),
    'access' => 'administer pane settings',
  );

  $groups['pane'] = array(
    'title' => t('Panes'),
    'access' => 'administer panes',
  );

  return $groups;
}

/**
 * Implements hook_variable_type_info().
 */
function pane_variable_type_info() {
  $type = array();
  $type['pane_part'] = array(
    'title' => t('Pane parts'),
    'options' => array('title' => t('Title'), 'body' => t('Body')),
  );
  $type['pane_content'] = array(
    'title' => t('Pane content'),
    'multiple' => array('title' => t('Title'), 'body' => t('Body')),
    'build callback' => 'pane_build_pane_content',
    'localize' => TRUE,
    'type' => 'multiple',
  );
  return $type;
}

/**
 * Helper that builds pane content form element grouping
 */
function pane_build_pane_content($variable, $options = array()) {
  $name = str_replace('[pane_part]', '', $variable['name']);
  $variable['children'][$name .'title']['type'] = 'string';
  $variable['children'][$name .'body']['type'] = 'text_format';
  $variable = variable_build_multiple($variable, $options);
  return $variable;
}

/**
 * Implements hook_variable_settings_form_alter().
 */
function pane_variable_settings_form_alter(&$form, &$form_state, $form_id) {
  // Insert variables into vertical tabs
  if ($form_id == 'variable_group_form' && $form['#variable_group_form'] == 'pane') {
    $form['pane'] = array(
      '#type' => 'vertical_tabs',
    );
    foreach ($form['#variable_edit_form'] as $variable) {
      if ($form[$variable]['#type'] == 'fieldset') {
        $form[$variable]['#description'] = '<h1>'. $form[$variable]['#title'] .'</h1>';
      }
      $form[$variable]['#group'] = 'pane';
    }
  }
  elseif ($form_id == 'variable_group_form' && $form['#variable_group_form'] == 'pane_settings') {
    $form['pane_panes']['#rows'] = 20;
    $form['#submit'][] = 'pane_settings_form_submit';
  }
}

/**
 * Delete variable cache after updating pane_settings form
 * Make all panes translatable if i18n_variable enabled
 */
function pane_settings_form_submit(&$form, &$form_state) {
  if (module_exists('i18n_variable')) {
    $val = $form_state['values']['pane_panes'];
    $val = explode("\n", $val);
    $panes = array();
    foreach ($val as $row) {
      $parts = explode('|', $row);
      $panes[] = 'pane_'. $parts[0] .'_[pane_part]';
    }
    variable_set('variable_realm_list_language', $panes);
  }
  cache_clear_all('*', 'cache_variable', TRUE);
}
