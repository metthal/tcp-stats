function initMenuActions() {
	$("#menu-analyses").click(function() {
		$("#menu-analyses").addClass("active");
		$("#menu-about").removeClass("active");
		$("#analysis-container").fadeIn();
		$("#about-container").hide();
	});

	$("#menu-about").click(function() {
		$("#menu-about").addClass("active");
		$("#menu-analyses").removeClass("active");
		$("#about-container").fadeIn();
		$("#analysis-container").hide();
	});
}

function initAnalysis() {
	$(".analysis").each(function() {
		var body = $(this).children(".analysis-body");
		var arrow = $(this).find(".analysis-header-arrow");

		$(body).hide();

		$(this).click(function() {
			if ($(body).is(":visible")) {
				$(body).slideUp();
				$(arrow).removeClass("rotate-down");
				$(arrow).addClass("rotate-up");
			}
			else {
				$(body).slideDown();
				$(arrow).removeClass("rotate-up");
				$(arrow).addClass("rotate-down");
			}
		});
	});
}

function initAbout() {
	$("#about-container").hide();
}

$(function() {
	initMenuActions();
	initAnalysis();
	initAbout();
});
