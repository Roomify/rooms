Feature: Availability filter
  In order to test overlapping confirmation message

@api @javascript
Scenario: Placing bookings in cart with the same dates
  Given unit types:
  | type     | label    | base_price | min_sleeps | max_sleeps | min_children | max_children |
  | standard | Standard | 100        | 2          | 3          | 0            | 1            |

  Given "standard" units:
  | name       |
  | Normal     |

  Given CommerceCart Availability Filter is not active

  Given the cache has been cleared

  Given I am logged in as a user with the "access checkout" permission
  Then I will be able to make a booking for "Normal" unit from "2016-01-05" to "2016-01-08"

  When I am on "booking/2016-01-05/2016-01-08/1/?rooms_group_size1=2"
  Then I select "1" from "standard[300][quantity]"
  And I wait for AJAX to finish
  And I press "Place Booking"
  Then I should see "Booking for Standard (3 Nights; Arrival: 05-01-2016 Departure: 08-01-2016)"
  And I should see "$300.00"

  When I am on "booking/2016-01-05/2016-01-08/1/?rooms_group_size1=2"
  Then I select "1" from "standard[300][quantity]"
  And I wait for AJAX to finish
  And I press "Place Booking"
  Then I should see "You have a booking request with overlapping dates, continuing will replace it."
  And I should see "Go Back to Search"
  And I press "Continue"
  Then I should see "Booking for Standard (3 Nights; Arrival: 05-01-2016 Departure: 08-01-2016)"
  And I should see "$300.00"

@api @javascript
Scenario: Placing bookings in cart with overlapping dates
  Given unit types:
  | type     | label    | base_price | min_sleeps | max_sleeps | min_children | max_children |
  | standard | Standard | 100        | 2          | 3          | 0            | 1            |

  Given "standard" units:
  | name       |
  | Normal     |

  Given CommerceCart Availability Filter is not active

  Given the cache has been cleared

  Given I am logged in as a user with the "access checkout" permission
  Then I will be able to make a booking for "Normal" unit from "2016-01-05" to "2016-01-08"

  When I am on "booking/2016-01-05/2016-01-08/1/?rooms_group_size1=2"
  Then I select "1" from "standard[300][quantity]"
  And I wait for AJAX to finish
  And I press "Place Booking"
  Then I should see "Booking for Standard (3 Nights; Arrival: 05-01-2016 Departure: 08-01-2016)"
  And I should see "$300.00"

  When I am on "booking/2016-01-03/2016-01-11/1/?rooms_group_size1=2"
  Then I select "1" from "standard[800][quantity]"
  And I wait for AJAX to finish
  And I press "Place Booking"
  Then I should see "You have a booking request with overlapping dates, continuing will replace it."
  And I should see "Go Back to Search"
  And I press "Continue"

@api @javascript
Scenario: Replacing overlapping bookings
  Given unit types:
  | type     | label    | base_price | min_sleeps | max_sleeps | min_children | max_children |
  | standard | Standard | 100        | 2          | 3          | 0            | 1            |

  Given "standard" units:
  | name       |
  | Normal     |

  Given CommerceCart Availability Filter is not active

  Given the cache has been cleared

  Given I am logged in as a user with the "access checkout" permission
  Then I will be able to make a booking for "Normal" unit from "2016-01-05" to "2016-01-08"

  When I am on "booking/2016-01-05/2016-01-08/1/?rooms_group_size1=2"
  Then I select "1" from "standard[300][quantity]"
  And I wait for AJAX to finish
  And I press "Place Booking"
  Then I should see "Booking for Standard (3 Nights; Arrival: 05-01-2016 Departure: 08-01-2016)"
  And I should see "$300.00"

  When I am on "booking/2016-01-03/2016-01-11/1/?rooms_group_size1=2"
  Then I select "1" from "standard[800][quantity]"
  And I wait for AJAX to finish
  And I press "Place Booking"
  Then I should see "You have a booking request with overlapping dates, continuing will replace it."
  And I should see "Go Back to Search"
  And I press "Continue"
  Then I should see "Booking for Standard (8 Nights; Arrival: 03-01-2016 Departure: 11-01-2016)"
  And I should see "$800.00"
  And I should not see "Booking for Standard (3 Nights; Arrival: 05-01-2016 Departure: 08-01-2016)"
  And I should not see "$300.00"
