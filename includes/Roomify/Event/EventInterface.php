<?php

namespace Roomify\Event;

/**
 * A standard interface for dealing with events in Roomify
 *
 * @author Ronald Ashri <ronald@bluesparklabs.com>
 */
interface EventInterface{

  /**
   * Returns the start time (hour / minute) for the event. Should set default value if none defined (e.g. the check in
   * time for a hotel).
   *
   * @param String $format How the $start_time should be formatted
   *
   * @return String $start_time
   */
  function getStartTime($format);

  /**
   * Returns the end time (hour / minute) for the event. Should set default value if none defined (e.g. the checkout
   * time for a hotel).
   *
   * @param String $format How the $end_time should be formatted
   *
   * @return String $end_time
   */
  function getEndTime($format);

  /**
   * Returns the start day for the event.
   *
   * @param String $format How the $start_day should be formatted
   *
   * @return String $start_day
   */
  function getStartDay($format);

  /**
   * Returns the end day for the event.
   *
   * @param String $format How the $end_day should be formatted
   *
   * @return String $end_day
   */
  function getEndDay($format);

  /**
   * Returns the start month for the event.
   *
   * @param String $format How the $start_month should be formatted
   *
   * @return String $start_month
   */
  function getStartMonth($format);

  /**
   * Returns the end Month for the event.
   *
   * @param String $format How the $end_month should be formatted
   *
   * @return String $end_month
   */
  function getEndMonth($format);

  /**
   * Returns the start Year for the event.
   *
   * @param String $format How the $start_year should be formatted
   *
   * @return String $start_year
   */
  function getStartYear($format);

  /**
   * Returns the end Year for the event.
   *
   * @param String $format How the $end_year should be formatted
   *
   * @return String $end_year
   */
  function getEndYear($format);

  /**
   * Returns the start day for the event.
   *
   * @param String $format How the $start_day should be formatted
   *
   * @return String $start_day
   */
  function getStartDate($format);

  /**
   * Returns the end time for the event. It should be 1159am if none defined
   *
   * @param String $format How the $end_time should be formatted
   *
   * @return String $end_time representing end time
   */
  function getEndDate($format);

  /**
   * @return Boolean true if event starts and ends on same day
   */
  function isSameDay();

  /**
   * @return Boolean true if event starts and ends in the same month
   */
  function isSameMonth();

  /**
   * @return Boolean true if event starts and ends in the same year
   */
  function isSameYear();

  /**
   * @return int the length of the event in seconds
   */
  function getSecondLength();

  /**
   * @return int across how many days does the event span
   */
  function getDayLength();

  /**
   * @return int across how many months does the event span
   */
  function getMonthLength();

  /**
   * @return int across how many years does the event span
   */
  function getYearLength();


}
