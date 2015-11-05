<?php
/**
 * Interface for all Entity Host plugins
 */
interface EntityHostInterface {
   /**
   * Adds a reference to the current Pane
   * 
   * @param pane_base $pane
   */
  public function setPane(&$pane);
  
  /**
   * Returns the current Pane
   *
   * @return pane_base
   */
  public function getPane();
  
  /**
   * Renders the current Entity
   * 
   * @return string
   */
  public function render();
  
  /**
   * Returns the edit form for the current Entity
   * 
   * @param array $form
   * @param array $form_state
   * @return array
   */
  public function edit_form($form, &$form_state);
  
  /**
   * Validates the edit form for the Entity
   * 
   * @param array $form
   * @param array $form_state
   */
  public function edit_form_validate($form, &$form_state);
  
  /**
   * Handles submission of the edit form for the Entity.
   * Returns the ID of the Entity.
   * 
   * @param array $form
   * @param array $form_state
   * @return integer
   */
  public function edit_form_submit($form, &$form_state);
}