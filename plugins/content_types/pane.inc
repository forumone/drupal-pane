<?php

/**
 * CTools content type definition
 */
$plugin = array(
  'title' => t('Custom text pane'),
  'content type' => 'pane_pane_content_type_content_type',
);

/**
 * List of Pane content types
 * 
 * @return array
 */
function pane_pane_content_type_content_types() {
  $types = array();
  
  foreach (pane_get_containers() as $container) {
    $info = array(
      'title' => $container->title,
      'category' => t('Custom panes'),
    );
    
    $types[$container->name] = $info;
  }
  
  return $types;
}

/**
 * Information about a particular pane content type
 * 
 * @param string $subtype_id
 * 
 * @return array
 */
function pane_pane_content_type_content_type($subtype_id) {
  $container = pane_load_container($subtype_id);
  $info = array();
  
  if (FALSE !== $container) {
    $info = array(
      'title' => $container->title,
      'category' => t('Custom panes'),  
    );
  }
  
  return $info;
}

/**
 * Renderer for Pane
 * 
 * @param string $subtype_id
 * @param array $conf
 * @param array $args
 * @param array $contexts
 * 
 * @return stdClass
 */
function pane_pane_content_type_render($subtype_id, $conf, $args, $contexts) {
  $container = pane_load_container($subtype_id);

  $block = new stdClass;

  if (FALSE !== $container) {
    $pane = pane_get_data($container);
    $block->title = $container->title;
    
    $block->content = array(
      '#markup' => $pane->render(),
      '#prefix' => '<div class = "'. str_replace('_', '-', substr($container->name, 0, -12)) .'">',
      '#suffix' => '</div>',
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
  $pane = pane_load_pane($conf['content']);
  
  if (FALSE !== $pane) {
    return t('Custom pane (%title)', array('%title' => $pane->title));
  }
  
  return NULL;
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
function pane_pane_content_type_edit_form($form, &$form_state) {
  $container = pane_load_container($form_state['subtype_name']);
  $plugin = pane_get_data($container);
  
  return $plugin->edit_form($form, $form_state);
}

/**
 * Form submit handler for Pane edit form
 * 
 * @param array $form
 * @param array $form_state
 */
function pane_pane_content_type_edit_form_submit(&$form, &$form_state) {
  $container = pane_load_container($form_state['subtype_name']);
  $plugin = pane_get_data($container);
  $plugin->edit_form_submit($form, $form_state);
  $plugin->save();
}
