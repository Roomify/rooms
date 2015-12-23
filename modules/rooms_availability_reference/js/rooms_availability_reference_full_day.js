(function ($) {

  Drupal.behaviors.rooms_availability_reference = {
    attach: function(context) {
      var today = moment();

      $('.cal').once('cal', function() {
        var lastSource;
        var cal_id = $(this).siblings('.availability-title').attr('id');

        $(this).fullCalendar({
          editable: false,
          dayNamesShort:[Drupal.t("Sun"), Drupal.t("Mon"), Drupal.t("Tue"), Drupal.t("Wed"), Drupal.t("Thu"), Drupal.t("Fri"), Drupal.t("Sat")],
          monthNames:[Drupal.t("January"), Drupal.t("February"), Drupal.t("March"), Drupal.t("April"), Drupal.t("May"), Drupal.t("June"), Drupal.t("July"), Drupal.t("August"), Drupal.t("September"), Drupal.t("October"), Drupal.t("November"), Drupal.t("December")],
          defaultDate: today,
          firstDay: Drupal.settings.roomsAvailabilityRef[cal_id].firstDay,
          header:{
            left: 'today',
            center: 'title',
            right: 'prev, next'
          },
          viewRender: function(view, element) {
            if (view.name == 'month') {
              view.calendar.removeEvents();

              var url = '?q=bat/v1/availability&units=' + Drupal.settings.roomsAvailabilityRef[cal_id].unitID.join() + '&start_date=' + view.intervalStart.get('year') + '-' + (view.intervalStart.get('month') + 1) + '-01&duration=1M';
              $.ajax({
                url: url,
                success: function(data) {
                  events = data['events'];

                  for (var index = 0; index < Drupal.settings.roomsAvailabilityRef[cal_id].unitID.length; index++) {
                    events_array = events[Drupal.settings.roomsAvailabilityRef[cal_id].unitID[index]];

                    view.calendar.addEventSource(events_array);
                  }
                }
              });
            }
          },
          eventRender: function(event, el, view) {
            // Remove Time from events.
            el.find('.fc-time').remove();
          },
          eventAfterRender: function(event, element, view) {
            // Hide events that are outside this month.
            if (event.start.month() != view.intervalStart.month()) {
              element.css('visibility', 'hidden');
              return;
            }
          }
        });

      });


      // Resize takes care of some quirks on occasion
      $(window).resize();

    }
  };
})(jQuery);
