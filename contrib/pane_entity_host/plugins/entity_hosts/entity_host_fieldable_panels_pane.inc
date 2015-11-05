<?php
/**
 * Provides an entity host for Fieldable Panels Panes
 */
class entity_host_fieldable_panels_pane extends entity_host_base implements EntityHostInterface {

  public function __construct() {
    ctools_form_include_file($form_state, drupal_get_path('module', 'fieldable_panels_panes') . '/plugins/content_types/fieldable_panels_pane.inc');
  }
  
  /**
   * Renders a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::render()
   */
  public function render() {
    $markup = '';
    
    if (FALSE === empty($this->pane->data['entity_id'])) {
      $entity_id = $this->pane->data['entity_id'];

      try {
        $entity = fieldable_panels_panes_load($entity_id);
      } catch ( Exception $e ) {
        watchdog(WATCHDOG_NOTICE, 'Unable to load Fieldable Panels Pane: @entity_id', array(
          '@entity_id' => $entity_id
        ));
        $entity = NULL;
      }
      
      if ($entity && ! empty($entity->fpid) && fieldable_panels_panes_access('view', $entity)) {
        $settings = field_bundle_settings('fieldable_panels_pane', $entity->bundle);
        $markup = fieldable_panels_panes_view($entity, $this->pane->configuration['view_mode']);
      }
    }
    
    return $markup;
  }

  /**
   * Provides edit form for a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::edit_form()
   */
  public function edit_form($form, &$form_state) {
    $entity_id = (TRUE === isset($this->pane->data['entity_id'])) ? $this->pane->data['entity_id'] : NULL;
    $bundle = (TRUE === isset($this->pane->configuration['bundle'])) ? $this->pane->configuration['bundle'] : NULL;
    
    if (! $entity_id) {
      $form_state['entity'] = fieldable_panels_panes_load_from_subtype($bundle);
    } else {
      $form_state['entity'] = fieldable_panels_panes_load_from_subtype('fpid:' . $entity_id);
    }
    
    $form = fieldable_panels_panes_entity_edit_form($form, $form_state);

    return $form;
  }

  /**
   * Validates the edit form for a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::edit_form_validate()
   */
  public function edit_form_validate($form, &$form_state) {
    if ($form_state['entity']) {
      fieldable_panels_panes_entity_edit_form_validate($form, $form_state);
    }
  }

  /**
   * Submit handler for the edit form for a Fieldable Panels Pane
   * 
   * @see EntityHostInterface::edit_form_submit()
   */
  public function edit_form_submit($form, &$form_state) {
    $entity = $form_state['entity'];
    $entity_id = NULL;
    
    if (! $entity) {
      return;
    }
    
    fieldable_panels_panes_entity_edit_form_submit($form, $form_state);
    
    if (!empty($entity->fpid)) {
      $entity_id = $entity->fpid;
    }
    
    return $entity_id;
  }
}
