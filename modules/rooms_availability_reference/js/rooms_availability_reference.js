(function ($) {


Drupal.behaviors.rooms_availability_reference = {
  attach: function(context) {
    var minDate =new Date();
    var i = 0;

    $('.cal').each(function() {
      $(this).fullCalendar({
        ignoreTimezone:false,
        editable:false,
        month:minDate.getMonth(),
        year:minDate.getFullYear(),
        header:{
          left: 'today',
  				center: 'title',
  				right: 'prev, next'
        },
        events: Drupal.settings.basePath + '?q=rooms/units/unit/'
                                          + Drupal.settings.roomsAvailabilityRef[i].unitID
                                          + '/availability/json/'
                                          + Drupal.settings.roomsAvailabilityRef[i].startyear
                                          + '/'
                                          + Drupal.settings.roomsAvailabilityRef[i].startmonth
                                          +'/1/' //start day
                                          + Drupal.settings.roomsAvailabilityRef[i].endyear
                                          +'/'
                                          + Drupal.settings.roomsAvailabilityRef[i].endmonth
                                          +'/'
                                          + Drupal.settings.roomsAvailabilityRef[i].endday
                                          + '/'
                                          + Drupal.settings.roomsAvailabilityRef[i].style
      });

      i++;
    });


    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
  
  

