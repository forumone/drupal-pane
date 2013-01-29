<?php
abstract class pane_base {
  protected $data;
  protected $lang;
  protected $container;
  
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
  public function save() {
    $current_data = $this->load_data();
    $current_data->data = serialize($this->data);
    $current_data->name = $current_data->container . '_' . $current_data->lang;
    
    ctools_export_crud_save('pane_data', $current_data);
    
    return $this;
  }
  
  /**
   * Loads the pane from the database or exported values
   * 
   * @return pane_base
   */
  public function load() {
    $current_data = $this->load_data();
    $this->data = $current_data->data;

    return $this;
  }
  
  /**
   * Loads the actual data from the database or exported values
   * 
   * @return array
   */
  protected function load_data() {
    ctools_include('export');
    
    $current_data = ctools_export_load_object('pane_data', 'conditions', array('container' => $this->container, 'lang' => $this->lang));
    
    if (0 < sizeof($current_data)) {
      $current_data = array_shift($current_data);
      if (0 < sizeof($current_data->data)) {
        $current_data->data = unserialize($current_data->data);
      }
    }
    else {
      $current_data = ctools_export_crud_new('pane_data');
      $current_data->container = $this->container;
      $current_data->lang = $this->lang;
    }
    
    return $current_data;
  }
  
  /**
   * Signature for edit form
   * 
   * @param array $form
   * @param array $form_state
   */
  public abstract function edit_form($form = array(), &$form_state = array());
  
  /**
   * Signature for edit form submit function
   * 
   * @param array $form
   * @param array $form_state
   */
  public abstract function edit_form_submit(&$form, &$form_state);
  
  /**
   * Signature for pane rendering
   */
  public abstract function render();
}