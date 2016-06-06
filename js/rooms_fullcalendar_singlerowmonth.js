(function ($) {

  var FC = $.fullCalendar;
  var View = FC.views.basic.class;
  var singleRowMonth;

  singleRowMonth = View.extend({
    computeRange: function(date) {
      var range = View.prototype.computeRange.call(this, date);
      range.end.add(30, 'days');

      return range;
    },

    renderDates: function() {
      this.dayNumbersVisible = true;
      this.dayGrid.numbersVisible = true;
      this.dayGrid.colHeadFormat = 'ddd';

      this.el.addClass('fc-basic-view').html(this.renderSkeletonHtml());
      this.renderHead();

      this.scroller.render();
      var dayGridContainerEl = this.scroller.el.addClass('fc-day-grid-container');
      var dayGridEl = $('<div class="fc-day-grid" />').appendTo(dayGridContainerEl);
      this.el.find('.fc-body > tr > td').append(dayGridContainerEl);

      this.dayGrid.setElement(dayGridEl);
      this.dayGrid.renderDates(this.hasRigidRows());
    },
  });

  FC.views.singleRowMonth = singleRowMonth;

})(jQuery);
