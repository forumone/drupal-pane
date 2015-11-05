<?php
class pane_entity_host extends pane_base {

  protected $host_plugin;

  public function __construct() {
    ctools_include('plugins');
  }
  
  /**
   * Returns the current host plugin
   * 
   * @return EntityHostInterface
   */
  protected function get_host_plugin() {
    if (! $this->host_plugin) {
      $plugins = ctools_get_plugins('pane_entity_host', 'plugins');

      if (FALSE === empty($this->configuration['entity'])) {
        // Use the entity_api plugin unless another is explicitly defined for
        // the entity type in question
        if (TRUE === isset($plugins[$this->configuration['entity']])) {
          $plugin_handler = $this->configuration['entity'];
        }
        else {
          $plugin_handler = 'entity_api';
        }

        $class = ctools_plugin_get_class($plugins[$plugin_handler], 'handler');
        
        if ($class) {
          $this->host_plugin = new $class();
          $this->host_plugin->setPane($this);
        }
      }
    }
    
    return $this->host_plugin;
  }

  /**
   * Provides the data edit form
   *
   * @see pane_base::data_edit_form()
   *
   * @return array
   */
  public function data_edit_form($form = array(), &$form_state = array()) {
    $plugin = $this->get_host_plugin();

    if ($plugin) {
      $form = $plugin->edit_form($form, $form_state);
    }
    
    return $form;
  }

  /**
   * Validates the form data
   *
   * @see pane_base::data_edit_form_validate()
   */
  public function data_edit_form_validate(&$form, &$form_state) {
    $plugin = $this->get_host_plugin();
    
    if ($plugin) {
      $plugin->edit_form_validate($form, $form_state);
    }
  }

  /**
   * Saves the form data
   *
   * @see pane_base::data_edit_form_submit()
   */
  public function data_edit_form_submit(&$form, &$form_state) {
    $plugin = $this->get_host_plugin();
    
    if ($plugin) {
      $this->data['entity_id'] = $plugin->edit_form_submit($form, $form_state);
    }
  }

  /**
   * Renders the pane text
   *
   * @see pane_base::render()
   *
   * @return string
   */
  public function render() {
    $markup = '';
    
    $plugin = $this->get_host_plugin();
    
    if ($plugin) {
      $markup = $plugin->render();
    }
    
    return $markup;
  }

  /**
   * Provides the container edit form
   *
   * @see pane_base::container_edit_form()
   *
   * @return array
   */
  public function container_edit_form(&$form_state = array()) {
    $form = array();
    $entities = array();
    $display_modes = array();
    $bundles = array();
    
    // Get a list of entities with host plugins
    $handled_entities = array_intersect_key(entity_get_info(), ctools_get_plugins('pane_entity_host', 'plugins'));

    $plugins = ctools_get_plugins('pane_entity_host', 'plugins');
    foreach (entity_get_info() as $name => $info) {
      $entities[$name] = t($info['label']);
      
      foreach ($info['view modes'] as $machine_name => $mode_info) {
        $display_modes[$name][$machine_name] = $mode_info['label'];
      }
      
      foreach ($info['bundles'] as $bundle => $bundle_info) {
        $bundles[$name][$bundle] = t($bundle_info['label']);
      }
    }
    
    $entity = (TRUE === isset($this->configuration['entity'])) ? $this->configuration['entity'] : array_shift(array_keys($entities));
    
    $form['entity'] = array(
      '#type' => 'select',
      '#title' => t('Entity'),
      '#default_value' => $entity,
      '#options' => $entities,
      '#ajax' => array(
        'callback' => 'pane_plugin_change_ajax',
        'wrapper' => 'pane-config'
      )
    );
    
    $form['bundle'] = array(
      '#type' => 'select',
      '#title' => t('Bundle'),
      '#options' => $bundles[$entity],
      '#default_value' => (TRUE === isset($this->configuration['bundle'])) ? $this->configuration['bundle'] : array()
    );
    
    $form['view_mode'] = array(
      '#type' => 'select',
      '#title' => t('View Mode'),
      '#options' => $display_modes[$entity],
      '#description' => t('Display mode used to render selections'),
      '#default_value' => (TRUE === isset($this->configuration['view_mode'])) ? $this->configuration['view_mode'] : '',
      '#required' => TRUE
    );
    
    return $form;
  }

  /**
   * Submit handler for container edit form
   *
   * @see pane_base::container_edit_form_submit()
   */
  public function container_edit_form_submit(&$form_state) {
    $this->configurationiguration = array();
    
    if (TRUE === isset($form_state['values']['configuration']['entity'])) {
      $this->configurationiguration['entity'] = $form_state['values']['configuration']['entity'];
    }
    
    if (TRUE === isset($form_state['values']['configuration']['view_mode'])) {
      $this->configurationiguration['view_mode'] = (string)$form_state['values']['configuration']['view_mode'];
    }
    
    if (TRUE === isset($form_state['values']['configuration']['bundle'])) {
      $this->configurationiguration['bundle'] = (string)$form_state['values']['configuration']['bundle'];
    }
    
    if (FALSE === empty($this->configurationiguration)) {
      $this->configuration = $this->configurationiguration;
    }
  }
}
