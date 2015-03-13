Feature: Once rooms_pricing is installed
  In order to create bookable units
  As a site administrator
  I should be able to manage the bookable units pricing options

@api @javascript
Scenario: Availability manager user should be able to modify the pricing of units both in bulk pricing page and per unit.
  #Creating a unit type and some units.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  |name       |base_price|
  |Normal     |120       |
  |Special    |130       |

  # Check the initial conditions.
  Given the cache has been cleared
  And I am logged in as a user with the "access administration pages,view any rooms_unit entity of bundle standard,administer rooms_unit pricing,update pricing any rooms_unit entity of bundle standard" permission
  Then the price for "Normal" between "2014-05-05" and "2014-07-07" should be "120"
  And the price for "Special" between "2014-05-05" and "2014-07-07" should be "130"

  # Check the Add to price operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Add to price" from "operation"
  And I fill in "amount" with "30"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "150"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "160"

  # Check the Add to price per night operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Add to price per night" from "operation"
  And I fill in "amount" with "10"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "200"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "210"

  # Check the Subtract from price operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Subtract from price" from "operation"
  And I fill in "amount" with "30"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "170"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "180"

  # Check the Subtract from price per night operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Subtract from price per night" from "operation"
  And I fill in "amount" with "10"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "120"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "130"

  # Check the Replace price operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Replace price" from "operation"
  And I fill in "amount" with "150"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "150"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "150"

  # Check the Increase price by % amount operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Increase price by % amount" from "operation"
  And I fill in "amount" with "20"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "180"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "180"

  # Check the Decrease price by % amount operation.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Decrease price by % amount" from "operation"
  And I fill in "amount" with "20"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-23" should be "144"
  And the price for "Special" between "2014-05-19" and "2014-05-23" should be "144"

  # Check the day of the week select boxes.
  Given I am on "admin/rooms/units/bulk_pricing_management/2014/5"
  Then I click "Update Pricing"
  And I fill in "rooms_start_date[date]" with "19/05/2014"
  And I fill in "rooms_end_date[date]" with "23/05/2014"
  And I select "Replace price" from "operation"
  And I fill in "amount" with "120"
  And I check the box "day_options[2]"
  And I check the box "day_options[3]"
  And I check the box "day_options[4]"
  And I select "All (this page)" from "select-all"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-19" and "2014-05-21" should be "120"
  And the price for "Special" between "2014-05-19" and "2014-05-21" should be "120"
  And the price for "Normal" between "2014-05-22" and "2014-05-23" should be "144"
  And the price for "Special" between "2014-05-22" and "2014-05-23" should be "144"

  # Check the single unit price management page.
  Given I am managing the "Normal" unit pricing
  Then I click "Update Unit Pricing"
  And I fill in "rooms_start_date[date]" with "29/05/2014"
  And I fill in "rooms_end_date[date]" with "23/06/2015"
  And I select "Replace price" from "operation"
  And I fill in "amount" with "150"
  And I press the "Update Unit Pricing" button

  Then the price for "Normal" between "2014-05-29" and "2015-06-23" should be "150"
