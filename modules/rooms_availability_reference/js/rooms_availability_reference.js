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
                                          + Drupal.settings[i].roomsAvailabilityRef.unitID
                                          + '/availability/json/'
                                          + Drupal.settings[i].roomsAvailabilityRef.startyear
                                          + '/'
                                          + Drupal.settings[i].roomsAvailabilityRef.startmonth
                                          +'/1/' //start day
                                          + Drupal.settings[i].roomsAvailabilityRef.endyear
                                          +'/'
                                          + Drupal.settings[i].roomsAvailabilityRef.endmonth
                                          +'/'
                                          + Drupal.settings[i].roomsAvailabilityRef.endday
                                          + '/'
                                          + Drupal.settings[i].roomsAvailabilityRef.style
      });

      i++;
    });


    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
  
  

