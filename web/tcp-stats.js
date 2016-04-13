function initDataDisplay() {
	if (typeof(tcpstats_data) !== 'undefined') {
		$("#no-analysis-found").remove();
	}
	else {
		$("#loading-data").remove();
	}
}

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

function initAbout() {
	$("#about-container").hide();
}

function displayAnalysesData() {
	if (typeof(tcpstats_data) === "undefined") {
		return;
	}

	$.each(tcpstats_data["analyses"], function(index, value) {
		displayAnalysisData(value, tcpstats_data.analyses_data[value.id]);
	});
}

function displayAnalysisData(metadata, data) {
	var analysis = $("<div></div>").addClass("analysis");
	var analysisHeader = $("<div></div>").addClass("analysis-header").appendTo(analysis);
	var analysisBody = $("<div></div>").addClass("analysis-body").appendTo(analysis);

	analysisHeader.text(metadata.name);

	var headerArrow = $("<span></span>").addClass("glyphicon").addClass("glyphicon-menu-up")
		.addClass("pull-right").addClass("analysis-header-arrow");
	headerArrow.appendTo(analysisHeader);

	switch (metadata.dataType) {
		case "key_value":
			createKeyValueData(data).appendTo(analysisBody);
			break;
		case "graph":
			createGraph(data).appendTo(analysisBody);
			break;
	}

	analysis.appendTo("#analysis-container");
}

function createKeyValueData(data) {
	var table = $("<table></table>").addClass("table").addClass("table-striped").addClass("key-value-table");
	for (i = 0; i < data.length; i++) {
		var row = $("<tr></tr>");
		$("<td></td>").text(data[i].key).appendTo(row);
		$("<td></td>").text(data[i].value).appendTo(row);
		row.appendTo(table);
	}

	return table;
}

function createGraph(data) {
	var container = $("<div></div>").addClass("container");
	for (i = 0; i < data.length; i++) {
		var graph = $("<div></div>").addClass("graph");
		data[i].chart.width = $("#analysis-container").width() - 50;
		$(graph).highcharts(data[i]);
		graph.appendTo(container);
	}

	return container;
}

function setupAnalysisHandlers() {
	$(".analysis").each(function() {
		var header = $(this).children(".analysis-header");
		var body = $(this).children(".analysis-body");
		var arrow = $(this).find(".analysis-header-arrow");

		$(body).hide();

		$(header).click(function() {
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

function postLoadingSetup() {
	displayAnalysesData();
	setupAnalysisHandlers();
	$("#loading-data").remove();
	$(".analysis").each(function(index, analysis) {
		$(analysis).stop(true, true).animate({ height: "toggle", opacity: "toggle" }, 400);
	});
}

$(function() {
	initDataDisplay();
	initMenuActions();
	initAbout();

	setTimeout(postLoadingSetup, 1000);
});
