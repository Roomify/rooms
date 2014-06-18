Feature: Have Rooms correctly installed
  In order to use Rooms
  As a site administrator
  I need to be able to access all the Rooms configuration screens

@api
Scenario: I can access the Bookable Units interface
  Given I am logged in as a user with the "administrator" role
  When I am on "admin/rooms"
  Then I should see the text "Bookable Units"
