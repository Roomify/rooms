Feature: Once rooms_availability is installed
  In order to create bookable units
  As a site administrator
  I should be able to manage the bookable units availability options

@api @javascript
Scenario: Availability manager user should be able to modify the availability state of units both in bulk availability page and per unit.
  # Creating a unit type and some units.
  Given unit types:
  | type     | label    | base_price | min_sleeps | max_sleeps | min_children | max_children |
  | standard | Standard | 100        | 2          | 3          | 0            | 1            |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  | name        | default_state |
  | Unavailable | 0             |
  | Available   | 1             |
  | On Request  | 2             |
  | An. Booking | 3             |

  # Checking the initial conditions.
  Given the cache has been cleared
  And I am logged in as a user with the "access administration pages,view any rooms_unit entity of bundle standard,administer rooms_unit availability,update availability any rooms_unit entity of bundle standard,access availability index service" permissions
  Then the state for "Unavailable" between "2016-01-05" and "2016-03-07" should be "0"
  And the state for "Available" between "2016-01-05" and "2016-03-07" should be "1"
  And the state for "On Request" between "2016-01-05" and "2016-03-07" should be "2"
  And the state for "An. Booking" between "2016-01-05" and "2016-03-07" should be "3"

  Given I am on "admin/rooms/units/bulk_unit_management/2016/1"
  Then I click "Update Availability"
  And I fill in "rooms_start_date[date]" with "19/01/2016"
  And I fill in "rooms_end_date[date]" with "23/01/2016"
  And I select "On Request" from "change_event_status"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Availability" button

  Then the state for "Available" between "2016-01-19" and "2016-01-23" should be "2"
  And the state for "Unavailable" between "2016-01-19" and "2016-01-23" should be "2"
  And the state for "On Request" between "2016-01-19" and "2016-01-23" should be "2"
  And the state for "An. Booking" between "2016-01-19" and "2016-01-23" should be "2"

  # Checking the single unit availability management page.
  Given I am managing the "Unavailable" unit availability
  Then I click "Update Unit Availability"
  And I fill in "rooms_start_date[date]" with "29/01/2016"
  And I fill in "rooms_end_date[date]" with "23/02/2017"
  And I select "Available" from "unit_state"
  And I press the "Update Availability" button

  Then the state for "Unavailable" between "2016-01-29" and "2017-02-23" should be "1"
