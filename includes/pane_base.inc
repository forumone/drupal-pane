<?php
abstract class pane_base {
  protected $data;
  protected $lang;
  protected $container;
  protected $configuration;
  protected $plugin;
  protected $title;
  protected $description;
  
  public function __get($name) {
    return (TRUE === isset($this->{$name})) ? $this->{$name} : NULL;
  }
  
  public function setPlugin($plugin) {
    $this->plugin = (string) $plugin;
    return $this;
  }
  
  public function getPlugin() {
    return $this->plugin;
  }
  
  public function setTitle($title) {
    $this->title = (string) $title;
    return $this;
  }
  
  public function getTitle() {
    return $this->title;
  }
  
  public function setDescription($description) {
    $this->description = (string) $description;
    return $this;
  }
  
  /**
   * Setter for the pane data
   * 
   * @param mixed $data
   * 
   * @return pane_base
   */
  public function setData($data) {
    $this->data = $data;
    return $this;
  }
  
  /**
   * Returns the pane data
   * 
   * @return mixed
   */
  public function getData() {
    return $this->data;
  }
  
  public function setConfiguration($configuration) {
    $this->configuration = $configuration;
    return $this;
  }
  
  public function getConfiguration() {
    return $this->configuration;
  }
  
  public function getConfigurationValue($key) {
    return (TRUE === isset($this->configuration[$key])) ? $this->configuration[$key] : NULL;
  }
  
  /**
   * Sets the pane language
   * 
   * @param string $lang
   * 
   * @return pane_base
   */
  public function setLang($lang) {
    $this->lang = (string) $lang;
    return $this;
  }
  
  /**
   * Returns the pane language
   * 
   * @return string
   */
  public function getLang() {
    return $this->lang;
  }
  
  /**
   * Sets the pane container machine name
   * 
   * @param string $container
   * 
   * @return pane_base
   */
  public function setContainer($container) {
    $this->container = (string) $container;
    return $this;
  }
  
  /**
   * Returns the pane container machine name
   * 
   * @return string
   */
  public function getContainer() {
    return $this->container;
  }
  
  /**
   * Saves the pane to the database
   * 
   * @return pane_base
   */
  public function save_data() {
    $current_data = $this->fetch_data();
    $current_data->name = $this->container . '_' . $current_data->lang;
    $current_data->data = $this->data;
    $current_data->container = $this->container;
    
    ctools_export_crud_save('pane_data', $current_data);
    
    return $this;
  }
  
  /**
   * Loads the pane from the database or exported values
   * 
   * @return pane_base
   */
  public function load_data() {
    $current_data = $this->fetch_data();
    $this->data = $current_data->data;

    return $this;
  }
  
  /**
   * Loads the actual data from the database or exported values
   * 
   * @return array
   */
  protected function fetch_data() {
    ctools_include('export');
    
    $current_data = ctools_export_load_object('pane_data', 'conditions', array('container' => $this->container, 'lang' => $this->lang));
    
    if (0 < sizeof($current_data)) {
      $current_data = array_shift($current_data);
    }
    else {
      $current_data = ctools_export_crud_new('pane_data');
      $current_data->container = $this->container;
      $current_data->lang = $this->lang;
    }
    
    return $current_data;
  }
  
  /**
   * Signature for data edit form
   * 
   * @param array $form
   * @param array $form_state
   * 
   * @return array
   */
  public abstract function data_edit_form($form = array(), &$form_state = array());
  
  /**
   * Signature for data edit form validate function
   *
   * @param array $form
   * @param array $form_state
   */
  public abstract function data_edit_form_validate(&$form, &$form_state);
  
  /**
   * Signature for data edit form submit function
   * 
   * @param array $form
   * @param array $form_state
   */
  public abstract function data_edit_form_submit(&$form, &$form_state);
  
  /**
   * Signature for pane rendering
   * 
   * @return string
   */
  public abstract function render();
  
  /**
   * Signature for container edit form
   * 
   * @param array $form
   * @param array $form_state
   * 
   * @return array
   */
  public abstract function container_edit_form(&$form_state = array());
  
  /**
   * Signature for container edit form submit function
   *
   * @param array $form
   * @param array $form_state
   */
  public abstract function container_edit_form_submit(&$form_state);
}