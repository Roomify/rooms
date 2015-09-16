Feature: Create Example Bookings
  In order to create bookings
  As a site administrator
  I need to be able to access all the Rooms booking configuration screens
  And create bookings from scratch

@api @javascript
Scenario: Bulk creation of units and bookings

	Given customer profiles:
  | profile_id | name        | country | locality | address       | postal_code |
  | profile1   | User test 1 | US      | Austin   | 1900 David St | 78705       |

	Given unit types:
  | type     | label    | base_price | min_sleeps | max_sleeps | min_children | max_children |
  | standard | Standard | 100        | 2          | 3          | 0            | 1            |
  | special  | Special  | 120        | 2          | 3          | 0            | 1            |
  | deluxe   | Deluxe   | 150        | 2          | 3          | 0            | 1            |
  | suite    | Suite    | 200        | 2          | 3          | 0            | 1            |
  | single   | Single   | 140        | 1          | 1          | 0            | 1            |

  Given 2 units of type "standard"
  Given 2 units of type "special"
  Given 2 units of type "deluxe"
  Given 2 units of type "suite"
  Given 2 units of type "single"

  Given 2 bookings of type "standard_booking" for all "standard" units
  Given 2 bookings of type "standard_booking" for all "special" units
  Given 2 bookings of type "standard_booking" for all "deluxe" units
  Given 2 bookings of type "standard_booking" for all "suite" units
  Given 2 bookings of type "standard_booking" for all "single" units
