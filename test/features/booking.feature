Feature: Once rooms_booking correctly installed
  In order to create bookings
  As a site administrator
  I need to be able to access all the Rooms booking configuration screens
  And create bookings from scratch

@api @javascript
Scenario: I can create a booking
  Given I am logged in as a user with the "administrator" role

  #Creating a unit type and some units.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  |name       |base_price|min_sleeps|max_sleeps|
  |Normal     |120       |1         |2         |
  |Special    |130       |2         |3         |

  # Checking that Standard booking type exists.
  When I am on "admin/rooms/bookings/booking-types"
  Then I should see the text "Standard Booking"

  # Creating a booking and acustomer profile in modal click by click.
  When I am on "admin/rooms/bookings"
  Then I should see the text "Add a Booking"

  When I click "Add a Booking"
  Then I should see the text "Add Standard Booking"

  When I click "create a new profile"
  And I wait for AJAX to finish

  Then I fill in "commerce_customer_address[und][0][name_line]" with "User test"
  And select "United States" from "commerce_customer_address[und][0][country]"
  And I wait for AJAX to finish

  Then I fill in "commerce_customer_address[und][0][thoroughfare]" with "1900 David St"
  And I fill in "commerce_customer_address[und][0][locality]" with "Austin"
  And select "Texas" from "commerce_customer_address[und][0][administrative_area]"
  And I fill in "commerce_customer_address[und][0][postal_code]" with "78705"
  And I press the "Save profile" button

  Given I wait for AJAX to finish
  Then I fill in "rooms_start_date[date]" with "19/06/2015"
  And I fill in "rooms_end_date[date]" with "23/06/2015"
  And I press the "Check availability" button
  And I wait for AJAX to finish

  Then select "Standard" from "unit_type"
  And I wait for AJAX to finish

  Then I should see the text "Normal - Cost: \$ 480"
  And I should see the text "Special - Cost: \$ 520"

  #Playing with the number of guests to check restrictions.
  Then select "1" from "data[group_size]"
  And I wait for AJAX to finish

  Then I should see the text "Normal - Cost: \$ 480"
  And I should see the text "Special - Cost: \$ 520"

  Then select "3" from "data[group_size]"
  And I wait for AJAX to finish

  Then I should not see the text "Normal - Cost: \$ 480"
  And I should see the text "Special - Cost: \$ 520"

  Then select "2" from "data[group_size]"
  And I wait for AJAX to finish

  Then I should see the text "Normal - Cost: \$ 480"
  And I should see the text "Special - Cost: \$ 520"

  # Selecting the desired unit.
  Then I select the radio button "Normal - Cost: $ 480"
  And I wait for AJAX to finish

  Then the "price" field should contain "480.00"
  And I press the "Save Booking" button

  # Checking that the unit has been locked.
  Then the "Normal" unit should be Unconfirmed by the last booking between "2015-06-29" and "2015-06-22"

  # Editing the booking to mark it as confirmed.
  When I am on "admin/rooms/bookings"
  Then I should see the text "19-06-2015"
  And I should see the text "23-06-2015"
  And I should see the text "User test"
  And I click "Edit"

  Then the "rooms_start_date[date]" field should contain "19/06/2015"
  And the "rooms_end_date[date]" field should contain "23/06/2015"

  Then I check the box "Booking Confirmed"
  And I press the "Save Booking" button

  # Checking that the unit has been locked and the booking confirmed..
  Then the "Normal" unit should be Confirmed by the last booking between "2015-06-29" and "2015-06-22"

  # Editing the booking and changing the unit assigned.
  When I am on "admin/rooms/bookings"
  And I click "Edit"

  Then I press the "Re-assign Unit" button
  And I wait for AJAX to finish

  Then select "Standard" from "unit_type"
  And I wait for AJAX to finish

  Then I select the radio button "Special - Cost: $ 520"
  And I wait for AJAX to finish

  Then the "price" field should contain "520.00"
  And I press the "Save Booking" button

  # Checking that the prior unit has been released and the new one locked.
  Then the "Special" unit should be Confirmed by the last booking between "2015-06-29" and "2015-06-22"
  And the state for "Normal" between "2015-06-19" and "2015-06-23" should be "1"

  # Deleting created entities to keep installation clean.
  When I am on "admin/rooms/bookings"
  And I click "Delete"
  Then I press the "Delete" button

  When I am on "admin/commerce/customer-profiles"
  And I click "delete"
  When I press the "Delete" button
  Then I should see the success message "The profile has been deleted."

@api @javascript
Scenario: I can create a booking programmatically
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