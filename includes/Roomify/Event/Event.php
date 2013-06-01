<?php

namespace Roomify\Event;


/**
 * Abstract implementation of an Event
 */
abstract class Event implements EventInterface {

  /** @var  Date - the start date for the event */
  public $start_date;

  /** @var Date - the end date (if nightly the last day the room is still not available) */
  public $end_date;

  /** @var  BookableUnit - the bookable unit associated with this event */
  public $unit;

  /** @var  EventState - the state associated with this event */
  public $state;

  function getStartTime($format = 'G:i') {
    return $this->start_date->format($format);
  }

  function getEndTime($format = 'G:i') {
    return $this->end_date->format($format);
  }

  function getStartDay($format = '') {

  }

  function getEndDay($format = '') {

  }

  function getStartMonth($format = '') {

  }

  function getEndMonth($format = '') {

  }

  function getStartYear($format = '') {

  }

  function getEndYear($format = '') {

  }


}