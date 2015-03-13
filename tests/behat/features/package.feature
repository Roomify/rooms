Feature: Once rooms_package correctly installed
  In order to create packages
  As a site administrator
  I need to be able to access all the Rooms packages configuration screens
  And create packages and bookings from scratch

@api @javascript
Scenario: I can create a booking
  Given I am logged in as a user with the "administrator" role

  #Creating a unit type and some units.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |
  |deluxe   |Deluxe   |150       |1         |4         |0           |0           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  |name       |base_price|min_sleeps|max_sleeps|
  |Normal     |120       |1         |2         |
  |Special    |130       |2         |3         |

  # Creating a bunch of units programmatically.
  Given "deluxe" units:
  |name       |base_price|min_sleeps|max_sleeps|
  |Suite      |120       |1         |2         |

  # Checking that Standard booking type exists.
  When I should be able to edit a "rooms_package" product
  Then I fill in "sku" with "test_package_all"
  And I fill in "title" with "Package All"
  And I fill in "commerce_price[und][0][amount]" with "100"
  And I fill in "rooms_package_dates[und][0][value][date]" with "05/05/2015"
  And I fill in "rooms_package_dates[und][0][value2][date]" with "05/15/2015"
  And I check the box "rooms_package_all_units[und]"
  And I press the "Save product" button

  When I should be able to edit a "rooms_package" product
  Then I fill in "sku" with "test_package_normal"
  And I fill in "title" with "Package Normal"
  And I fill in "commerce_price[und][0][amount]" with "120"
  And I fill in "rooms_package_dates[und][0][value][date]" with "04/05/2015"
  And I fill in "rooms_package_dates[und][0][value2][date]" with "04/15/2015"
  And I check the box "Standard"
  And I press the "Save product" button

  When I should be able to edit a "rooms_package" product
  Then I fill in "sku" with "test_package_deluxe"
  And I fill in "title" with "Package Deluxe"
  And I fill in "commerce_price[und][0][amount]" with "180"
  And I fill in "rooms_package_dates[und][0][value][date]" with "04/05/2015"
  And I fill in "rooms_package_dates[und][0][value2][date]" with "04/15/2015"
  And I check the box "Deluxe"
  And I press the "Save product" button

  When I should be able to edit a "rooms_package" product
  Then I fill in "sku" with "test_package_suite"
  And I fill in "title" with "Package Suite"
  And I fill in "commerce_price[und][0][amount]" with "180"
  And I fill in "rooms_package_dates[und][0][value][date]" with "04/05/2015"
  And I fill in "rooms_package_dates[und][0][value2][date]" with "04/15/2015"
  And I select the "Suite" room for package
  And I press the "Save product" button

  Then I should be able to edit a "rooms_package" node
  And I fill in "title" with "Package All"
  And I select "test_package_all: Package All" from "rooms_package_product[und]"
  And I press the "Save" button
  Then I should see the button "Add to cart"

  Then I should be able to edit a "rooms_package" node
  And I fill in "title" with "Package Normal"
  And I select "test_package_normal: Package Normal" from "rooms_package_product[und]"
  And I press the "Save" button
  Then I should see the button "Add to cart"

  Then I should be able to edit a "rooms_package" node
  And I fill in "title" with "Package Deluxe"
  And I select "test_package_deluxe: Package Deluxe" from "rooms_package_product[und]"
  And I press the "Save" button
  Then I should see the button "Add to cart"

  Then I should be able to edit a "rooms_package" node
  And I fill in "title" with "Package Suite"
  And I select "test_package_suite: Package Suite" from "rooms_package_product[und]"
  And I press the "Save" button
  Then I should see the button "Add to cart"

  Given I am viewing the package "Package Suite"
  Then I should see the button "Add to cart"
  And I press the "Add to cart" button

  Then I should be on "cart"
  And I should see the text "Booking for package Package Suite start date: 2015-04-05 end date: 2015-04-15 for unit Suite"
  And I should see the text "180.00"

  Then I press the "Checkout" button
  And I fill in "customer_profile_billing[commerce_customer_address][und][0][name_line]" with "User test"
  And I fill billing address with "<random>", "Test City", "CA", "94806", "US"

  And I press the "Continue to next step" button
  Then I should see the text "Review order"
  And I press the "Continue to next step" button
  Then I should see the text "Checkout complete"

  When I am viewing the package "Package Suite"
  Then the "Out of stock" button is disabled

  When I am viewing the package "Package Deluxe"
  Then the "Out of stock" button is disabled

  When I am viewing the package "Package Normal"
  Then I should see the button "Add to cart"
  And I press the "Add to cart" button

  Then I should be on "cart"
  And I should see the text "Booking for package Package Normal start date: 2015-04-05 end date: 2015-04-15"
  And I should see the text "120"

  Then I press the "Checkout" button
  And I fill in "customer_profile_billing[commerce_customer_address][und][0][name_line]" with "User test"
  And I fill billing address with "<random>", "Test City", "CA", "94806", "US"

  And I press the "Continue to next step" button
  Then I should see the text "Review order"
  And I press the "Continue to next step" button
  Then I should see the text "Checkout complete"

  When I am viewing the package "Package Normal"
  Then I should see the button "Add to cart"
  And I press the "Add to cart" button

  Then I should be on "cart"
  And I should see the text "Booking for package Package Normal start date: 2015-04-05 end date: 2015-04-15"
  And I should see the text "120"

  Then I press the "Checkout" button
  And I fill in "customer_profile_billing[commerce_customer_address][und][0][name_line]" with "User test"
  And I fill billing address with "<random>", "Test City", "CA", "94806", "US"

  And I press the "Continue to next step" button
  Then I should see the text "Review order"
  And I press the "Continue to next step" button
  Then I should see the text "Checkout complete"

  When I am viewing the package "Package Normal"
  Then the "Out of stock" button is disabled

  When I am viewing the package "Package All"
  Then I should see the button "Add to cart"

  When I am on "admin/commerce/products"
  Then I click "Edit" in the "test_package_all" row
  And I fill in "rooms_package_dates[und][0][value][date]" with "04/05/2015"
  And I fill in "rooms_package_dates[und][0][value2][date]" with "04/15/2015"
  And I press the "Save product" button

  When I am viewing the package "Package All"
  Then the "Out of stock" button is disabled

  When I am on "admin/commerce/products"
  Then I click "Edit" in the "test_package_all" row
  And I fill in "rooms_package_dates[und][0][value][date]" with "06/05/2015"
  And I fill in "rooms_package_dates[und][0][value2][date]" with "06/15/2015"
  And I press the "Save product" button

  When I am viewing the package "Package All"
  Then I should see the button "Add to cart"
  And I press the "Add to cart" button

  Then I should be on "cart"
  And I should see the text "Booking for package Package All start date: 2015-06-05 end date: 2015-06-15"
  And I should see the text "100"

  Then I press the "Checkout" button
  And I fill in "customer_profile_billing[commerce_customer_address][und][0][name_line]" with "User test"
  And I fill billing address with "<random>", "Test City", "CA", "94806", "US"

  And I press the "Continue to next step" button
  Then I should see the text "Review order"
  And I press the "Continue to next step" button
  Then I should see the text "Checkout complete"

  When I am on "admin/rooms/bookings"
  Then I should see values in row table:
  |Customer |Arrival   |Departure |Unit   |
  |User test|05-04-2015|15-04-2015|Suite  |
  |User test|05-04-2015|15-04-2015|Normal |
  |User test|05-04-2015|15-04-2015|Special|
  |User test|05-06-2015|15-06-2015|       |