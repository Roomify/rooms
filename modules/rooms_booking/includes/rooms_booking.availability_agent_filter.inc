<?php


/**
 * An availability agent filter receives a set of units and applies a filter
 * to them returning the remainder.
 */
interface AvailabilityAgentFilter {
  
  /**
   *@param array rooms The set of rooms to filter through
   */
  public function __construct($units, $parameters);
  
  /**
   *@return array The rooms remaining following the application of the filter
   */
  public function applyFilter();
  
}



class AvailabilityAgentSizeFilter implements AvailabilityAgentFilter {
  
  protected $units;
  
  protected $parameters;
  
  public function __construct($units, $parameters) {
    
  }
  
  public function applyFilter() {
    
    // Search for suitbale units
    
    return $units;
  }
  
}




class AvailabilityAgentDateFilter implements AvailabilityAgentFilter {
  
  protected $units;
  
  protected $parameters;
  
  public function __construct($units, $parameters) {
    
  }
  
  public function applyFilter() {
    
    // Search for suitbale units
    
    return $units;
  }
  
}



class AvailabilityAgentCommerceFilter implements AvailabilityAgentFilter {
  
  protected $units;
  
  protected $parameters;
  
  public function __construct($units, $parameters) {
    
  }
  
  public function applyFilter() {
    
    // Search for suitbale units
    
    return $units;
  }
  
}


