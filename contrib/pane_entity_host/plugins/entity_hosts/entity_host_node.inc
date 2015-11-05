<?php
/**
 * Provides an entity host for Nodes
 */
class entity_host_node extends entity_host_base implements EntityHostInterface {

  /**
   * Renders a Node
   * 
   * @see EntityHostInterface::render()
   */
  public function render() {
    $markup = '';
    
    if (FALSE === empty($this->pane->data['entity_id'])) {
      $entity_id = $this->pane->data['entity_id'];

      try {
        $entity = node_load($entity_id);
      } catch ( Exception $e ) {
        watchdog(WATCHDOG_NOTICE, 'Unable to load Node: @entity_id', array(
          '@entity_id' => $entity_id
        ));
        $entity = NULL;
      }
      
      if ($entity && ! empty($entity->nid) && node_access('view', $entity)) {
        $markup = node_view($entity, $this->pane->configuration['view_mode']);
      }
    }
    
    return $markup;
  }

  protected function create_default_node($bundle) {
    $info = entity_get_info('node');
    $bundle_key = $info['entity keys']['bundle'];
    
    // Make a stub entity of the right bundle to pass to the entity_ui_get_form().
    $values = array(
      $bundle_key => $bundle,
    );
    
    $entity = entity_create('node', $values);
    
    return $entity;
  }
  /**
   * Provides edit form for a Node
   * 
   * @see EntityHostInterface::edit_form()
   */
  public function edit_form($form, &$form_state) {
    $entity_id = (TRUE === isset($this->pane->data['entity_id'])) ? $this->pane->data['entity_id'] : NULL;
    $bundle = (TRUE === isset($this->pane->configuration['bundle'])) ? $this->pane->configuration['bundle'] : NULL;
    $entity_type = (TRUE === isset($this->pane->configuration['entity'])) ? $this->pane->configuration['entity'] : NULL;
    
    if ($entity_type) {
      $entity = NULL;
      
      if ($entity_id) {
        try {
          $entity = node_load($entity_id);
        }
        catch (Exception $e) {
          watchdog(WATCHDOG_NOTICE, 'Unable to load Node: @entity_id', array(
            '@entity_id' => $entity_id
          ));
        }
      }
      
      if (!$entity) {
        $entity = $this->create_default_node();
      }
      
      $extra_fields = field_info_extra_fields('node', $entity->type, 'form');
      if (!isset($entity->title)) {
        $node->title = NULL;
      }
      node_object_prepare($entity);
      
      $form['title'] = array(
        '#type' => 'textfield',
        '#title' => check_plain('Title'),
        '#required' => TRUE,
        '#default_value' => $entity->title,
        '#maxlength' => 255,
        // The label might be missing if the Title module has replaced it.
        '#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -5,
      );
      
      $form['status'] = array(
        '#type' => 'radios',
        '#access' => user_access('administer nodes'),
        '#title' => t('Status'),
        '#default_value' => $entity->status,
        '#options' => array(1 => t('Published'), 0 => t('Unpublished')),
        '#required' => TRUE,
        '#weight' => 99,
      );
      
      $langcode = entity_language('node', $entity);
      field_attach_form('node', $entity, $form, $form_state, $langcode);
    }
    
    return $form;
  }

  /**
   * Validates the edit form for a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::edit_form_validate()
   */
  public function edit_form_validate($form, &$form_state) {
    $entity_id = (TRUE === isset($this->pane->data['entity_id'])) ? $this->pane->data['entity_id'] : NULL;
    $bundle = (TRUE === isset($this->pane->configuration['bundle'])) ? $this->pane->configuration['bundle'] : NULL;
    $entity_type = (TRUE === isset($this->pane->configuration['entity'])) ? $this->pane->configuration['entity'] : NULL;
    
    if ($entity_id) {
      $entity = node_load($entity_id);
    }
    else {
      $entity = $this->create_default_node($bundle);
    }
    
    $info = entity_get_info($entity_type);
    $entity->type = $bundle;
    $entity_values = drupal_array_get_nested_value($form_state['values'], $form['#parents']);
    
    // Copy top-level form values that are not for fields to entity properties,
    // without changing existing entity properties that are not being edited by
    // this form. Copying field values must be done using field_attach_submit().
    $values_excluding_fields = $info['fieldable'] ? array_diff_key($entity_values, field_info_instances($entity_type, $bundle)) : $entity_values;
    foreach ($values_excluding_fields as $key => $value) {
      $entity->$key = $value;
    }
    
    if ($info['fieldable']) {
      field_attach_submit($entity_type, $entity, $form_state['values'], $form_state);
    }
    
    $node = node_submit($form_state['values']);
    
    $child_form_state = form_state_defaults();
    $child_form_state['values'] = drupal_array_get_nested_value($form_state['values'], $form['#parents']);
    foreach (module_implements('node_submit') as $module) {
      $function = $module . '_node_submit';
      $function($entity_form['#entity'], $form, $child_form_state);
    }
    
    return $node->nid;
  }

  /**
   * Submit handler for the edit form for a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::edit_form_submit()
   */
  public function edit_form_submit($form, &$form_state) {
    // TODO: Fix submit
  }
}