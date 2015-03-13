Feature: Once rooms_availability_reference is installed
  In order to create bookable units
  As a site administrator
  I should be able to add Availability calendars in any entity.

@api @javascript
Scenario: Admin user should be able to add an availability_calendar in nodes.
  Given I am logged in as a user with the "administrator" role

   #Creating a unit type and some units.
  Given unit types:
    |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
    |standard |Standard |100       |2         |3         |0           |1           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
    |name       |base_price|
    |Normal     |120       |
    |Special    |130       |

  # Creating a customer profile.
  Given customer profiles:
    |profile_id |name        |country |locality |address      |postal_code|
    |profile1   |User test 1 |US      |Austin   |1900 David St|78705      |
    |profile2   |User test 2 |US      |Austin   |1900 David St|78705      |

  # Creating a booking.
  Given "standard_booking" bookings:
    |profile_id|guests|children|start_date|end_date  |unit   |status|
    |profile2  |1     |1       |2015-05-19|2015-05-23|Normal |1     |

  Then the "Normal" unit should be Confirmed by the last booking between "2015-05-19" and "2015-05-22"

  # Creating a booking.
  Given "standard_booking" bookings:
    |profile_id|guests|children|start_date|end_date  |unit   |status|
    |profile1  |3     |0       |2015-05-19|2015-05-23|Special|0     |

  Then the "Special" unit should be Unconfirmed by the last booking between "2015-05-19" and "2015-05-22"

  When I add the "availability_ref" availability reference field referencing to "standard" units in "page2" content
  Then I should be able to edit a "page2" node
  And reference units "Normal,Special" in the "availability_ref" field
  And I fill in "title" with "asdf"
  And I press the "Save" button

  Then I navigate in the fullCalendar to "2015-05"
  And I should see the text "User test 2"
  And I should see the text "UNCONF"

