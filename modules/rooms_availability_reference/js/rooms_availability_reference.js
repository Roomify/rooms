(function ($) {


Drupal.behaviors.rooms_availability_reference = {
  attach: function(context) {
    var minDate =new Date();
    var i = 0;

    $('.cal').once('cal', function() {
      var j = i;
      var lastSource;

      $(this).fullCalendar({
        ignoreTimezone:false,
        editable:false,
        dayNamesShort:[Drupal.t("Sun"), Drupal.t("Mon"), Drupal.t("Tue"), Drupal.t("Wed"), Drupal.t("Thu"), Drupal.t("Fri"), Drupal.t("Sat")],
        monthNames:[Drupal.t("January"), Drupal.t("February"), Drupal.t("March"), Drupal.t("April"), Drupal.t("May"), Drupal.t("June"), Drupal.t("July"), Drupal.t("August"), Drupal.t("September"), Drupal.t("October"), Drupal.t("November"), Drupal.t("December")],
        month:minDate.getMonth(),
        year:minDate.getFullYear(),
        header:{
          left: 'today',
          center: 'title',
          right: 'prev, next'
        },
        viewDisplay: function(view) {
          if (view.name == 'month') {
            for (var url in lastSource) {
              view.calendar.removeEventSource(url);
            }
            view.calendar.refetchEvents();

            lastSource = [];
            for (var index = 0; index < Drupal.settings.roomsAvailabilityRef.length; index++) {
              url = '?q=rooms/units/unit/' + Drupal.settings.roomsAvailabilityRef[index].unitID + '/availability/json/'
                + view.start.getFullYear() + '/' + (view.start.getMonth() + 1) + '/1/' //start day
                + view.end.getFullYear() +'/' + (view.end.getMonth() + 1) +'/0/' // end day
                + Drupal.settings.roomsAvailabilityRef[index].style;

                view.calendar.addEventSource(url);

                lastSource[index] = url;
            }

          }
        }
      });

      i++;
    });


    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
