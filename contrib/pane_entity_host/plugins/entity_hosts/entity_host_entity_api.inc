<?php
/**
 * Provides an entity host for EntityAPI
 */
class entity_host_entity_api extends entity_host_base implements EntityHostInterface {

  /**
   * Renders an Entity
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
        watchdog(WATCHDOG_NOTICE, 'Unable to load Entity: @entity_id', array(
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

  /**
   * Provides edit form for an Entity
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
          $entity = entity_load_single($entity_type, $entity_id);
        }
        catch (Exception $e) {
          watchdog(WATCHDOG_NOTICE, 'Unable to load Entity: @entity_id', array(
            '@entity_id' => $entity_id
          ));
        }
      }
      
      if (!$entity) {
        $form = entity_ui_get_bundle_add_form($entity_type, $bundle);
      }
      else {
        $form = entity_form($entity_type, $entity);
      }
    }
    
    return $form;
  }

  /**
   * Validates the edit form for a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::edit_form_validate()
   */
  public function edit_form_validate($form, &$form_state) {
    // TODO: Fix validate
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