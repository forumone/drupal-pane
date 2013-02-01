<?php
class pane_entity_reference extends pane_base {
  const BY_BUNDLE = 'bundle';
  const BY_VIEW = 'view';
  
  /**
   * Provides the dataedit form
   * 
   * @see pane_base::data_edit_form()
   * 
   * @return array
   */
  public function data_edit_form($form = array(), &$form_state = array()) {
    $form['description'] = array(
      '#type' => 'text_format',
      '#title' => t('Description'),
      '#default_value' => (TRUE === isset($this->data['value'])) ? $this->data['value'] : '',
      '#format' => (TRUE === isset($this->data['format'])) ? $this->data['format'] : ''
    );
    
    return $form;
  }
  
  /**
   * Saves the form data
   * 
   * @see pane_base::data_edit_form_submit()
   */
  public function data_edit_form_submit(&$form, &$form_state) {
    $this->data = $form_state['values']['description'];
  }
  
  /**
   * Renders the pane text
   * 
   * @see pane_base::render()
   * 
   * @return string
   */
  public function render() {
    $text = '';
    
    if (TRUE === isset($this->data['value'])) {
      $text = check_markup($this->data['value'], $this->data['format']);
    }
    
    return $text;
  }
  
  public function container_edit_form(&$form_state = array()) {
    $mode = (TRUE === isset($this->configuration['mode'])) ? $this->configuration['mode'] : self::BY_BUNDLE;
    
    $form[$this->container . '_mode'] = array(
      '#type' => 'select',
      '#title' => t('Mode'),
      '#default_value' => $mode,
      '#options' => array(
        self::BY_BUNDLE => t('Filter by bundle'),
        self::BY_VIEW => t('Filter by view')
      ),
    );
    
    switch ($mode) {
      case self::BY_BUNDLE:
        $form += $this->get_bundle_form($form_state);
        break;
        
      case self::BY_VIEW:
        $form += $this->get_view_form($form_state);
        break;
    }
    
    return $form;
  }
  
  protected function get_bundle_form(&$form_state) {
    $form = array();
    
    $entities = array();
    $bundles = array();
    
    foreach (entity_get_info() as $name => $info) {
      $entities[$name] = t($info['label']);
      
      foreach ($info['bundles'] as $bundle => $bundle_info) {
        $bundles[$name][$bundle] = t($bundle_info['label']);
      }
    }
    
    $entity = (TRUE === isset($this->configuration['entity'])) ? $this->configuration['entity'] : array_shift(array_keys($entities));
    
    $form[$this->container . '_entity'] = array(
      '#type' => 'select',
      '#title' => t('Entity'),
      '#default_value' => $entity,
      '#options' => $entities,
      '#ajax' => array(
        'callback' => 'pane_plugin_change_ajax',
        'wrapper' => $this->container . '-config',
      ),
    );
    
    $form[$this->container . '_target_bundles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Target bundles'),
      '#options' => $bundles[$entity],
      '#description' => t('The bundles of the entity type that can be referenced. Optional, leave empty for all bundles.'),
      '#process' => array('form_process_checkboxes'),
      '#theme_wrappers' => array('checkboxes'),
      '#default_value' => (TRUE === isset($this->configuration['target_bundles'])) ? $this->configuration['target_bundles'] : array(),
    );
    
    return $form;
  }
  
  protected function get_view_form(&$form_state) {
    $form = array();
    
    return $form;
  }
  
  public function container_edit_form_submit(&$form_state) {
    $configuration = array();
    
    if (TRUE === isset($form_state['values'][$this->container . '_mode'])) {
      $configuration['mode'] = $form_state['values'][$this->container . '_mode'];
    }
    
    if (TRUE === isset($form_state['values'][$this->container . '_entity'])) {
      $configuration['entity'] = $form_state['values'][$this->container . '_entity'];
    }
    
    if (TRUE === isset($form_state['values'][$this->container . '_target_bundles'])) {
      $configuration['target_bundles'] = array_filter($form_state['values'][$this->container . '_target_bundles']);
    }
    
    if (FALSE === empty($configuration)) {
      $this->configuration = $configuration;
    }
  }
}