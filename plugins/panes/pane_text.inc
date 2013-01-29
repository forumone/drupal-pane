<?php
class pane_text extends pane_base {
  /**
   * Provides the edit form
   * 
   * @see pane_base::edit_form()
   * 
   * @return array
   */
  public function edit_form($form = array(), &$form_state = array()) {
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
   * @see pane_base::edit_form_submit()
   */
  public function edit_form_submit(&$form, &$form_state) {
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
}