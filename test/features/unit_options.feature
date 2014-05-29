Feature: Once rooms_unit and rooms_booking are installed
  In order to create bookable units with options
  As a site administrator
  I should be able to add options to unis and unit types and check that price is generated properly

@api @javascript
Scenario: Creating some units, adding options at both unit and unit type levels, then create bookings and check that everything works properly.
  #Creating a unit type programmatically.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  |name   |base_price|min_sleeps|max_sleeps|
  |Normal |120      |1         |2         |
  |Deluxe |140      |2         |4         |

  Given I am logged in as a user with the "administrator" role
  When I am on the "Normal" unit
  Then I should see the text "Normal"
  When I am on the "Deluxe" unit
  Then I should see the text "Deluxe"

  Given I am editing the "Normal" unit
  Then I fill in "rooms_booking_unit_options[und][0][name]" with "Champagne"
  And select "5" from "rooms_booking_unit_options[und][0][quantity]"
  And select "Add to price" from "rooms_booking_unit_options[und][0][operation]"
  And I fill in "rooms_booking_unit_options[und][0][value]" with "10"

  Then I press the "Add another item" button
  And I wait for AJAX to finish

  Then I fill in "rooms_booking_unit_options[und][1][name]" with "Breakfast"
  And select "1" from "rooms_booking_unit_options[und][1][quantity]"
  And select "Add to price per night" from "rooms_booking_unit_options[und][1][operation]"
  And I fill in "rooms_booking_unit_options[und][1][value]" with "12"

  Then I press the "Add another item" button
  And I wait for AJAX to finish

  Then I fill in "rooms_booking_unit_options[und][2][name]" with "Coupon"
  And select "3" from "rooms_booking_unit_options[und][2][quantity]"
  And select "Subtract from price" from "rooms_booking_unit_options[und][2][operation]"
  And I fill in "rooms_booking_unit_options[und][2][value]" with "15"

  Then I press the "Add another item" button
  And I wait for AJAX to finish

  Then I fill in "rooms_booking_unit_options[und][3][name]" with "No towels"
  And select "1" from "rooms_booking_unit_options[und][3][quantity]"
  And select "Subtract from price per night" from "rooms_booking_unit_options[und][3][operation]"
  And I fill in "rooms_booking_unit_options[und][3][value]" with "10"

  Then I press the "Add another item" button
  And I wait for AJAX to finish

  Then I fill in "rooms_booking_unit_options[und][4][name]" with "Special offer"
  And select "1" from "rooms_booking_unit_options[und][4][quantity]"
  And select "Replace price" from "rooms_booking_unit_options[und][4][operation]"
  And I fill in "rooms_booking_unit_options[und][4][value]" with "100"

  Then I press the "Add another item" button
  And I wait for AJAX to finish

  Then I fill in "rooms_booking_unit_options[und][5][name]" with "Extra bed"
  And select "1" from "rooms_booking_unit_options[und][5][quantity]"
  And select "Increase price by % amount" from "rooms_booking_unit_options[und][5][operation]"
  And I fill in "rooms_booking_unit_options[und][5][value]" with "20"

  Then I press the "Add another item" button
  And I wait for AJAX to finish

  Then I fill in "rooms_booking_unit_options[und][6][name]" with "Single use"
  And select "1" from "rooms_booking_unit_options[und][6][quantity]"
  And select "Decrease price by % amount" from "rooms_booking_unit_options[und][6][operation]"
  And I fill in "rooms_booking_unit_options[und][6][value]" with "15"

  Then I press the "Save Unit" button

  Given I am editing the "Normal" unit

  Then the "rooms_booking_unit_options[und][0][name]" field should contain "Champagne"
  And the "rooms_booking_unit_options[und][0][quantity]" field should contain "5"
  And the "rooms_booking_unit_options[und][0][operation]" field should contain "add"
  And the "rooms_booking_unit_options[und][0][value]" field should contain "10"

  And the "rooms_booking_unit_options[und][1][name]" field should contain "Breakfast"
  And the "rooms_booking_unit_options[und][1][quantity]" field should contain "1"
  And the "rooms_booking_unit_options[und][1][operation]" field should contain "add-daily"
  And the "rooms_booking_unit_options[und][1][value]" field should contain "12"

  And the "rooms_booking_unit_options[und][2][name]" field should contain "Coupon"
  And the "rooms_booking_unit_options[und][2][quantity]" field should contain "3"
  And the "rooms_booking_unit_options[und][2][operation]" field should contain "sub"
  And the "rooms_booking_unit_options[und][2][value]" field should contain "15"

  And the "rooms_booking_unit_options[und][3][name]" field should contain "No towels"
  And the "rooms_booking_unit_options[und][3][quantity]" field should contain "1"
  And the "rooms_booking_unit_options[und][3][operation]" field should contain "sub-daily"
  And the "rooms_booking_unit_options[und][3][value]" field should contain "10"

  And the "rooms_booking_unit_options[und][4][name]" field should contain "Special offer"
  And the "rooms_booking_unit_options[und][4][quantity]" field should contain "1"
  And the "rooms_booking_unit_options[und][4][operation]" field should contain "replace"
  And the "rooms_booking_unit_options[und][4][value]" field should contain "100"

  And the "rooms_booking_unit_options[und][5][name]" field should contain "Extra bed"
  And the "rooms_booking_unit_options[und][5][quantity]" field should contain "1"
  And the "rooms_booking_unit_options[und][5][operation]" field should contain "increase"
  And the "rooms_booking_unit_options[und][5][value]" field should contain "20"

  And the "rooms_booking_unit_options[und][6][name]" field should contain "Single use"
  And the "rooms_booking_unit_options[und][6][quantity]" field should contain "1"
  And the "rooms_booking_unit_options[und][6][operation]" field should contain "decrease"
  And the "rooms_booking_unit_options[und][6][value]" field should contain "15"

  #Creating a booking to play with it
  Given customer profiles:
  |profile_id |name      |country |locality |address      |postal_code|
  |profile    |User test |US      |Austin   |1900 David St|78705      |

  # Creating a booking.
  Given "standard_booking" bookings:
  |profile_id|guests|children|start_date|end_date  |unit   |status|
  |profile   |1     |1       |2015-05-19|2015-05-23|Normal |1     |

  Given I am on "admin/rooms/bookings"
  Then I should see the text "19-05-2015"
  And I should see the text "23-05-2015"
  And I should see the text "User test"
  And I click "Edit"

  Then I press the "Re-assign Unit" button
  And I wait for AJAX to finish

  Then select "Standard" from "unit_type"
  And I wait for AJAX to finish

  Then I select the radio button "Normal - Cost: $ 480"
  And I wait for AJAX to finish

  # Checking that all the options has been added.
  Then I should see the text "Available options for Normal"
  And I should see the text "Champagne"
  And I should see the text "Breakfast"
  And I should see the text "Coupon"
  And I should see the text "No towels"
  And I should see the text "Special offer"
  And I should see the text "Extra bed"
  And I should see the text "Single use"

  Then I check the box "Champagne"
  And I wait for AJAX to finish
  Then the "price" field should contain "490.00"

  Then I select "4" from "champagne:quantity"
  And I wait for AJAX to finish
  Then the "price" field should contain "520.00"

  Then I check the box "Breakfast"
  And I wait for AJAX to finish
  Then the "price" field should contain "568.00"

  Then I check the box "Coupon"
  And I wait for AJAX to finish
  Then the "price" field should contain "553.00"

  Then I select "3" from "coupon:quantity"
  And I wait for AJAX to finish
  Then the "price" field should contain "523.00"

  Then I uncheck the box "Coupon"
  And I wait for AJAX to finish
  Then the "price" field should contain "568.00"

  Then I check the box "No towels"
  And I wait for AJAX to finish
  Then the "price" field should contain "528.00"

  Then I uncheck the box "No towels"
  And I wait for AJAX to finish

  Then I uncheck the box "Breakfast"
  And I wait for AJAX to finish

  Then I uncheck the box "Champagne"
  And I wait for AJAX to finish

  Then the "price" field should contain "480.00"

  Then I check the box "Special offer"
  And I wait for AJAX to finish
  Then the "price" field should contain "100.00"

  Then I uncheck the box "Special offer"
  And I wait for AJAX to finish

  Then I check the box "Extra bed"
  And I wait for AJAX to finish
  Then the "price" field should contain "576.00"

  Then I uncheck the box "Extra bed"
  And I wait for AJAX to finish

  Then I check the box "Single use"
  And I wait for AJAX to finish
  Then the "price" field should contain "408.00"

  Then I uncheck the box "Single use"
  And I wait for AJAX to finish
  Then the "price" field should contain "480.00"

  Then I check the box "Breakfast"
  And I wait for AJAX to finish
  Then the "price" field should contain "528.00"

  Then I check the box "Coupon"
  And I wait for AJAX to finish

  Then I select "3" from "coupon:quantity"
  And I wait for AJAX to finish
  Then the "price" field should contain "483.00"

  And I press the "Save Booking" button

  # Checking data has been stored properly
  Given I am on "admin/rooms/bookings"
  And I click "Edit"

  Then I should see the text "Options: Breakfast : 1, Coupon : 3"
  And I should see the text "Currently assigned unit: Normal / Standard"
  And I should see the text "Price: \$ 483.00"

  Then I press the "Save Booking" button

  # Checking that options info persists (known bug)
  Given I am on "admin/rooms/bookings"
  And I click "Edit"

  Then I should see the text "Options: Breakfast : 1, Coupon : 3"
  And I should see the text "Currently assigned unit: Normal / Standard"
  And I should see the text "Price: \$ 483.00"

  # Adding some new options at unit type level.
  Given I am on "admin/rooms/units/unit-types/manage/standard"

  Then I fill in "rooms_booking_unit_options[und][0][name]" with "Scotch Whisky"
  And select "5" from "rooms_booking_unit_options[und][0][quantity]"
  And select "Add to price" from "rooms_booking_unit_options[und][0][operation]"
  And I fill in "rooms_booking_unit_options[und][0][value]" with "25"
  And I press the "Save unit type" button

  Given I am on "admin/rooms/units/unit-types/manage/standard"
  Then the "rooms_booking_unit_options[und][0][name]" field should contain "Scotch Whisky"
  And the "rooms_booking_unit_options[und][0][quantity]" field should contain "5"
  And the "rooms_booking_unit_options[und][0][operation]" field should contain "add"
  And the "rooms_booking_unit_options[und][0][value]" field should contain "25"

  Given I am on "admin/rooms/bookings"
  And I click "Edit"

  Then I press the "Re-assign Unit" button
  And I wait for AJAX to finish

  Then I should see the text "Available options for Normal"
  And I should see the text "Scotch Whisky"
  And I should see the text "Champagne"
  And I should see the text "Breakfast"
  And I should see the text "Coupon"
  And I should see the text "No towels"
  And I should see the text "Special offer"
  And I should see the text "Extra bed"
  And I should see the text "Single use"

  Then I select the radio button "Deluxe - Cost: $ 560"
  And I wait for AJAX to finish
  And I should see the text "Scotch Whisky"
  And I should not see the text "Champagne"
  And I should not see the text "No towels"
  And I should not see the text "Special offer"
  And I should not see the text "Extra bed"
  And I should not see the text "Single use"

  Then I check the box "Scotch Whisky"
  And I wait for AJAX to finish
  Then the "price" field should contain "585.00"

  Then I select "3" from "scotch_whisky:quantity"
  And I wait for AJAX to finish
  Then the "price" field should contain "635.00"

  Then I press the "Save Booking" button

   # Checking data has been stored properly
  Given I am on "admin/rooms/bookings"
  And I click "Edit"

  Then I should see the text "Options: Scotch Whisky : 3"
  And I should see the text "Currently assigned unit: Deluxe / Standard"
  And I should see the text "Price: \$ 635.00"

@api @javascript
Scenario: Adding options from tables for testing purposes.
  #Creating a unit type programmatically.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  |name   |base_price|min_sleeps|max_sleeps|
  |Normal |120      |1         |2         |
  |Deluxe |140      |2         |4         |

  Given options for "standard" unit type:
  |name         |quantity|operation|value|
  |Scotch Whisky|5       |add      |25   |

  Given options for "Normal" unit:
  |name         |quantity|operation|value|
  |Champagne    |5       |add      |10   |
  |Breakfast    |1       |add-daily|12    |
  |Coupon       |3       |sub      |15   |
  |No towels    |1       |sub-daily|10   |
  |Special offer|1       |replace  |100  |
  |Extra bed    |1       |increase |20   |
  |Single use   |1       |decrease |15   |

  #Creating a booking to play with it
  Given customer profiles:
  |profile_id |name      |country |locality |address      |postal_code|
  |profile    |User test |US      |Austin   |1900 David St|78705      |

  # Creating a booking.
  Given "standard_booking" bookings:
  |profile_id|guests|children|start_date|end_date  |unit   |status|
  |profile   |1     |0       |2015-05-19|2015-05-23|Normal |1     |

  Given I am logged in as a user with the "administrator" role

  # Editing the booking.
  Given I am on "admin/rooms/bookings"
  And I click "Edit"

  Then I press the "Re-assign Unit" button
  And I wait for AJAX to finish

  Then I should see the text "Available options for Normal"
  And I should see the text "Scotch Whisky"
  And I should see the text "Champagne"
  And I should see the text "Breakfast"
  And I should see the text "Coupon"
  And I should see the text "No towels"
  And I should see the text "Special offer"
  And I should see the text "Extra bed"
  And I should see the text "Single use"