Feature: Once rooms_unit is installed
  In order to create bookable units
  As a site administrator
  I should be able to access the unit type and unit edition pages

@api @javascript
Scenario: Unit type manager can access the Bookable unit types page and create, edit and delete Unit types
  Given I am logged in as a user with the "administer rooms_unit_type entities" permission
  When I am on "admin/rooms/units/unit-types"
  Then I should see the text "Add bookable unit type"
  When I click "Add bookable unit type"
  Then I am on "admin/rooms/units/unit-types/add"
  When I fill in "label" with "Standard"
  And I fill in "data[base_price]" with "100"
  And I fill in "data[min_sleeps]" with "2"
  And I fill in "data[max_sleeps]" with "3"
  And I fill in "data[min_children]" with "0"
  And I fill in "data[max_children]" with "1"
  When I press the "Save unit type" button
  Then I should be on "admin/rooms/units/unit-types"
  And I should see the text "Standard"
  And I should see the text "(Machine name: standard)"
  Then I am on "admin/rooms/units/unit-types/manage/standard"
  And the "label" field should contain "Standard"
  And the "data[base_price]" field should contain "100"
  And the "data[min_sleeps]" field should contain "2"
  And the "data[max_sleeps]" field should contain "3"
  And the "data[min_children]" field should contain "0"
  And the "data[max_children]" field should contain "1"
  Then I fill in "label" with "Modified value"
  And I fill in "data[base_price]" with "1200"
  And I fill in "data[min_sleeps]" with "1"
  And I fill in "data[max_sleeps]" with "4"
  And I fill in "data[min_children]" with "1"
  And I fill in "data[max_children]" with "3"
  When I press the "Save unit type" button
  Then I should be on "admin/rooms/units/unit-types"
  When I am on "admin/rooms/units/unit-types/manage/standard"
  And the "label" field should contain "Modified value"
  And the "data[base_price]" field should contain "1200"
  And the "data[min_sleeps]" field should contain "1"
  And the "data[max_sleeps]" field should contain "4"
  And the "data[min_children]" field should contain "1"
  And the "data[max_children]" field should contain "3"
  When I am on "admin/rooms/units/unit-types/manage/standard/delete"
  And I press the "Confirm" button
  Then I should see the message "Deleted Bookable Unit Type Modified value."

@api @javascript
Scenario: Unit type manager creates two unit types and two unit admin creates and edit it
  #Creating a unit type programmatically.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |
  |deluxe   |Deluxe   |150       |1         |2         |0           |1           |

  # Standard unit manager manages the unit type.
  Given the cache has been cleared
  And I am logged in as a user with the "access administration pages,view any rooms_unit entity of bundle standard,create rooms_unit entities of bundle standard,update any rooms_unit entity of bundle standard,delete any rooms_unit entity of bundle standard" permissions
  When I am on "admin/rooms/units/add/standard"
  Then the "base_price" field should contain "100"
  And the "min_sleeps" field should contain "2"
  And the "max_sleeps" field should contain "3"
  And the "min_children" field should contain "0"
  And the "max_children" field should contain "1"
  Then I fill in "name" with "Standard one"
  And I press the "Save Unit" button
  Then I should see the message "Bookable unit Standard one saved"
  When I am editing the "Standard one" unit
  Then I fill in "name" with "Standard one edited"
  And I fill in "base_price" with "150"
  And I fill in "min_sleeps" with "3"
  And I fill in "max_sleeps" with "8"
  And I fill in "min_children" with "1"
  And I fill in "max_children" with "4"
  And I press the "Save Unit" button
  Then I should see the message "Bookable unit Standard one edited saved"
  When I am on the "Standard one edited" unit
  Then I should see the text "Standard one edited"
  When I am editing the "Standard one edited" unit
  Then the "base_price" field should contain "150"
  And the "min_sleeps" field should contain "3"
  And the "max_sleeps" field should contain "8"
  And the "min_children" field should contain "1"
  And the "max_children" field should contain "4"
  When I am deleting the "Standard one edited" unit
  And I press the "Delete" button
  Then I should see the message "The unit Standard one edited has been deleted."

  # Deluxe unit manager manages the unit type.
  Given I am logged in as a user with the "access administration pages,view any rooms_unit entity of bundle deluxe,create rooms_unit entities of bundle deluxe,update any rooms_unit entity of bundle deluxe,delete any rooms_unit entity of bundle deluxe" permissions
  When I am on "admin/rooms/units/add/deluxe"
  Then the "base_price" field should contain "150"
  And the "min_sleeps" field should contain "1"
  And the "max_sleeps" field should contain "2"
  And the "min_children" field should contain "0"
  And the "max_children" field should contain "1"
  Then I fill in "name" with "Deluxe one"
  And I press the "Save Unit" button
  Then I should see the message "Bookable unit Deluxe one saved"
  When I am editing the "Deluxe one" unit
  Then I fill in "name" with "Deluxe one edited"
  And I fill in "base_price" with "150"
  And I fill in "min_sleeps" with "3"
  And I fill in "max_sleeps" with "8"
  And I fill in "min_children" with "1"
  And I fill in "max_children" with "4"
  And I press the "Save Unit" button
  Then I should see the message "Bookable unit Deluxe one edited saved"
  When I am on the "Deluxe one edited" unit
  Then I should see the text "Deluxe one edited"
  When I am editing the "Deluxe one edited" unit
  Then the "base_price" field should contain "150"
  And the "min_sleeps" field should contain "3"
  And the "max_sleeps" field should contain "8"
  And the "min_children" field should contain "1"
  And the "max_children" field should contain "4"
  When I am deleting the "Deluxe one edited" unit
  And I press the "Delete" button
  Then I should see the message "The unit Deluxe one edited has been deleted."

@api @javascript
Scenario: Creating a bunch of units for testing purposes.
  #Creating a unit type programmatically.
  Given unit types:
  |type     |label    |base_price|min_sleeps|max_sleeps|min_children|max_children|
  |standard |Standard |100       |2         |3         |0           |1           |

  # Creating a bunch of units programmatically.
  Given "standard" units:
  |name|base_price|min_sleeps|max_sleeps|
  |test1|120      |1         |2         |
  |test2|140      |2         |4         |

  Given I am logged in as a user with the "administrator" role
  When I am on the "test1" unit
  Then I should see the text "test1"
  When I am on the "test2" unit
  Then I should see the text "test2"
