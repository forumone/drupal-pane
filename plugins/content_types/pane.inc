<?php

$plugin = array(
  'title' => t('TCustom text pane'),
  'description' => t('Displays a custom text pane'),
  'single' => TRUE,
  'render callback' => 'pane_render',
  'edit form' => 'pane_edit_form',
  'admin title' => 'pane_admin_title',
  'admin info' => 'pane_admin_info',
  'category' => array(t('Custom Panes'), 0),
);

/**
 * Renderer for Pane
 * 
 * @param string $subtype
 * @param array $conf
 * @param array $args
 * @param array $contexts
 * 
 * @return stdClass
 */
function pane_render($subtype, $conf, $args, $contexts) {
  $content = pane_load_pane($conf['content']);
  $block = new stdClass;

  if (!empty($content['title'])) {
    $block->title = $content['title'];
  }
  if (!empty($content['body'])) {
    $block->content = array(
      '#markup' => check_markup($content['body']['value'], $content['body']['format']),
      '#prefix' => '<div class = "'. str_replace('_', '-', substr($conf['content'], 0, -12)) .'">',
      '#suffix' => '</div>',
    );
  }

  if (user_access('administer panes')) {
    $search = array('_', '[', ']');
    $replace = array('-', '', '');
    $fieldset_id = str_replace($search, $replace, $conf['content']);
    $block->admin_links = array(
      array(
        'title' => t('Edit content'),
        'href' => "admin/config/system/pane/$fieldset_id",
      ),
    );
  }

  return $block;
}

/**
 * Generate administrative title for Panes
 * 
 * @param string $subtype
 * @param array $conf
 * @param array $context
 * 
 * @return string
 */
function pane_admin_title($subtype, $conf, $context = NULL) {
  $variable = variable_get_info($conf['content']);
  return t('Custom pane (%title)', array('%title' => $variable['title']));
}

/**
 * Administrative information for Panes
 * 
 * @param string $subtype
 * @param array $conf
 * @param array $context
 * 
 * @return stdClass
 */
function pane_admin_info($subtype, $conf, $context = NULL) {
  return pane_render($subtype, $conf, NULL, $context);
}

/**
 * Edit form for Pane
 * 
 * @param array $form
 * @param array $form_state
 * 
 * @return array
 */
function pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  foreach (pane_get_panes() as $id => $title) {
    $options[$id .'_[pane_part]'] = $title;
  }

  $form['content'] = array(
    '#type' => 'select',
    '#title' => t('Select the content for this pane'),
    '#options' => $options,
    '#default_value' => $conf['content'],
  );

  return $form;

}

/**
 * Form submit handler for Pane edit form
 * 
 * @param array $form
 * @param array $form_state
 */
function pane_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['content'] = $form_state['values']['content'];
}
