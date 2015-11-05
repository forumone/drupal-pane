<?php
/**
 * Base class for Entity Host Panes
 */
class entity_host_base {
  protected $pane;
  
  /**
   * Adds a reference to the current Pane
   * 
   * @param pane_base $pane
   */
  public function setPane(&$pane) {
    $this->pane = $pane;
  }
  
  /**
   * Returns the current Pane
   * 
   * @return pane_base
   */
  public function getPane() {
    return $this->pane;
  }
}