(function ($) {


Drupal.behaviors.rooms_availability_reference = {
  attach: function(context) {
    var today = moment();

    $('.cal').once('cal', function() {
      var lastSource;
      var cal_id = $(this).siblings('.availability-title').attr('id');

      $(this).fullCalendar({
        ignoreTimezone:false,
        editable:false,
        dayNamesShort:[Drupal.t("Sun"), Drupal.t("Mon"), Drupal.t("Tue"), Drupal.t("Wed"), Drupal.t("Thu"), Drupal.t("Fri"), Drupal.t("Sat")],
        monthNames:[Drupal.t("January"), Drupal.t("February"), Drupal.t("March"), Drupal.t("April"), Drupal.t("May"), Drupal.t("June"), Drupal.t("July"), Drupal.t("August"), Drupal.t("September"), Drupal.t("October"), Drupal.t("November"), Drupal.t("December")],
        defaultDate: today,
        header:{
          left: 'today',
          center: 'title',
          right: 'prev, next'
        },
        viewRender: function(view) {
          if (view.name == 'month') {
            for (var url in lastSource) {
              view.calendar.removeEventSource(lastSource[url]);
            }
            view.calendar.refetchEvents();

            lastSource = [];
            for (var index = 0; index < Drupal.settings.roomsAvailabilityRef[cal_id].unitID.length; index++) {
              url = '?q=rooms/units/unit/' + Drupal.settings.roomsAvailabilityRef[cal_id].unitID[index] + '/availability/json/'
                + view.intervalStart.get('year') + '/' + (view.intervalStart.get('month') + 1);

                view.calendar.addEventSource(url);

                lastSource[index] = url;
            }

          }
        },
        //Remove Time from events
        eventRender: function(event, el) {
          el.find('.fc-time').remove();

          // Add a class if the event start it is not "AV" or "N/A".
          if (el.hasClass('fc-start') && this.id != 1 && this.id != 0) {
            el.append('<div class="event-start"/>');
            el.find('.event-start').css('border-top-color', this.color);
          }

          // Add a class if the event end and it is not "AV" or "N/A".
          if (el.hasClass('fc-end') && this.id != 1 && this.id != 0) {
            el.append('<div class="event-end"/>');
            el.find('.event-end').css('border-top-color', this.color);
          }
        },
        eventAfterRender: function( event, element, view ) {
          // Event width.
          var width = element.parent().width()
          // Event colspan number.
          var colspan = element.parent().get(0).colSpan;
          // Single cell width.
          var cell_width = width/colspan;
          var half_cell_width = cell_width/2;

          // Move events between table margins.
          element.css('margin-left', half_cell_width);
          element.css('margin-right', half_cell_width);

          // Calculate width event to add end date triangle.
          width_event = element.children('.fc-content').width();

          // Add a margin left to the top triangle.
          element.children().closest('.event-end').css('margin-left', width_event-22);

          // If the event end in a next row.
          if(element.hasClass('fc-not-end')) {
            element.css('margin-right', 0);
          }
          // If the event start in a previous row.
          if(element.hasClass('fc-not-start')) {
            element.css('margin-left', 0);
            element.children().closest('.event-end').css('margin-left', width_event);
          }
        }
      });

    });


    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
