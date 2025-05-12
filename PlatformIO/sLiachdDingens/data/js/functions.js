/*
  Projekt: sLiachdDingens-ESP8266
  Copyright © 2025 V0giK

  Dieses Programm ist freie Software: Du kannst es unter den Bedingungen
  der GNU General Public License, wie von der Free Software Foundation
  veröffentlicht, weitergeben und/oder modifizieren – entweder gemäß
  Version 3 der Lizenz oder (nach Deiner Wahl) einer späteren Version.

  Der vollständige Lizenztext ist in der Datei LICENSE enthalten:
  https://github.com/V0giK/sLiachdDingens-ESP8266/blob/main/LICENSE
*/

'use strict';

let gConfigObj;
let gSequenceObj;
let gPresetObj;

let gPortCnt; // wird aus EEPROM ausgelesen
let gSlotCnt; // wird aus EEPROM ausgelesen

let gFormSubmitted = false;
let gFormChanged = false;
let gFormPrev = false;
let gFormSubmittedConf = false;
let gFormChangedConf = false;


/**
 * incMenu.html in <div id="includedMenuContent"></div> laden
 */
$(function(){
	$("#includedMenuContent").load("incMenu.html"); 
});


/**
 * config.json in globale Variable gConfigObj einlesen
 */
function readConfig(){

	let xmlhttp = new XMLHttpRequest();
	xmlhttp.onload = function() {
		gConfigObj = JSON.parse(this.responseText);
	}
	xmlhttp.open("POST", "json/config.json", false);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();

	readSlotCnt();
	readPortCnt();

}

/**
 * SlotCnt in globale Variable gConfigObj hinzufügen
 */
 function readSlotCnt(){

	let xmlhttp = new XMLHttpRequest();
	xmlhttp.onload = function() {
		gSlotCnt = this.responseText;
	}
	xmlhttp.open("POST", "/getSlotCnt", false);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();
}

/**
 * PortCnt in globale Variable gPortCnt einlesen
 */
 function readPortCnt(){

	let xmlhttp = new XMLHttpRequest();
	xmlhttp.onload = function() {
		gPortCnt = this.responseText;
	}
	xmlhttp.open("POST", "/getPortCnt", false);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();
}

/**
 * lightProfil.json in globale Variable gSequenceObj einlesen
 */
function readLightSequence(){

	let xmlhttp = new XMLHttpRequest();
	xmlhttp.onload = function() {
		gSequenceObj = JSON.parse(this.responseText);
	}
	xmlhttp.open("POST", "json/lightProfil.json", false);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();
}

/**
 * lightPresets.json in globale Variable gPresetObj einlesen
 */
 function readLightPreset(){

	let xmlhttp = new XMLHttpRequest();
	xmlhttp.onload = function() {
		gPresetObj = JSON.parse(this.responseText);
	}
	xmlhttp.open("POST", "json/lightPresets.json", false);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();
}

/**
 * LightPorts erstellen und in HTML schreiben
 * - HTML für alle Slots erstellen (incl. Presets)
 * - HTML in ID "ports" schreiben
 * 
 * @param {*} seite - 'ports' | 'pwr' | ...
 */
function crtLightPorts(seite){

	let text = "";
	text += '<div class="w3-bar menuRow">';
    text += '<a class="w3-bar-item w3-button tablink w3-dark-grey" onclick="openProfil(event,\'D0\')">Profil 1</a>';
    text += '<a class="w3-bar-item w3-button tablink" onclick="openProfil(event,\'D1\')">Profil 2</a>';
    text += '<a class="w3-bar-item w3-button tablink" onclick="openProfil(event,\'D2\')">Profil 3</a>';
  	text += '</div>';

	for (let x in gSequenceObj["ports"]) {
		let val = {val:gSequenceObj["ports"][x]};
		text += crtPort(val, seite);
	}

	document.getElementById("ports").innerHTML = text;

	if(seite == 'pwr'){
		document.getElementById("LPP_LOW").value = gSequenceObj["lowPwrPercent"];
	}
}

/**
 * Profile ein/ausblenden
 * 
 * @param {*} evt 
 * @param {*} profilName 
 */
function openProfil(evt, profilName) {
	var i, x, tablinks;
	// ausblenden
	x = document.getElementsByClassName("LPorts");
	for (i = 0; i < x.length; i++) {
		x[i].style.display = "none";
	}
	tablinks = document.getElementsByClassName("tablink");
	for (i = 0; i < tablinks.length; i++) {
		tablinks[i].className = tablinks[i].className.replace(" w3-dark-grey", "");
	}
	// einblenden
	x = document.getElementsByClassName(profilName);
	for (i = 0; i < x.length; i++) {
		x[i].style.display = "block";
	}
	evt.currentTarget.className += " w3-dark-grey";
}


/**
 * LightPresets HTML erstellen
 * 
 * @param {*} id - ID der ComboBox
 * @param {*} selectVal - value bei dem 'selected' eingetragen wird
 */
function crtPresetHtml(id, selectVal){
	let selectTag = '<select class="select" title="default presets" onchange="doPreset(\'' + id + '\', this);">';
	let options = "";
	let selected = "";
	let hasSelected = false;

	for (let x in gPresetObj["presets"]) {
		if(gPresetObj["presets"][x].seq == selectVal){
			selected = "selected";
			hasSelected = true;
		}
		options += '<option value="' +  gPresetObj["presets"][x].seq + '" ' + selected + '>' + encodeHtml(gPresetObj["presets"][x].name) + '</option>';
		selected = "";
	}
	if(!hasSelected) selected = "selected";
	options = '<option value="" ' + selected + '>Preset laden ...</option>' + options;

	selectTag += options;
	selectTag += "</select>";

	// HTML der PresetCombobox zurückgeben
	return [selectTag, hasSelected];
}

/**
 * HTML für einen LightPort erstellen
 * 
 * @param {*} seq   - Alle Daten der Sequence
 * @param {*} seite - 'ports' | 'pwr' | ...
 * @returns - HTML String
 */
function crtPort(seq, seite){

	let elementId1 = 'LP' + seq.val.port;
	let fieldPref = "";
	
	if(seite == 'ports') {
		fieldPref = "S";
	} else if(seite == 'pwr') {
		fieldPref = "P";
	}
	
	let idLabel = "LPnameLabel_" + seq.val.port;
	let idLabelEdit = "LPname_" + seq.val.port;
	let dblClickLabel = ' ondblclick="document.getElementById(\'' + idLabelEdit + '\').style.display=\'block\';document.getElementById(\'' + idLabel + '\').style.display=\'none\';" title="Bezeichnung editieren" ';
	let text = '<div class="container_2572aa03">';
	text += '<div class="container_2572aa03_padding" >';
	text += '<div class="textstyle1">';
	text += '<div ' + dblClickLabel + ' style="cursor:pointer">';
	text += '<span class="textstyle4" style="display:block" id="' + idLabel + '">' + encodeHtml(seq.val.name) + ' <span class="w3-small">(Port: ' + seq.val.port + ')</span></span>';
	text += '<input class="LPNAME" style="display:none" type="text" value="'+encodeHtml(seq.val.name)+'" maxlength="35" name="' + idLabelEdit + '" id="' + idLabelEdit + '" onChange="setFormChanged(true);" title="">';
	text += '</div>';
	text += '<div class="form_sequence_padding" >';

	for(let prof=0; prof < seq.val.profile.length; prof++){
		let elementId = elementId1 + "P" + prof;
		let classDiv = "D"+prof;
		let cssDisplay = "block";
		if(prof != 0) cssDisplay = "none";
		text += '<div class="textstyle1 LPorts ' + classDiv + '" style="display:' + cssDisplay + '">';
		if(seite == 'ports') {
			let hasSelected = false;
			let cmb ="";
			[cmb, hasSelected] = crtPresetHtml(elementId+fieldPref, seq.val.profile[prof]["seq"].join(','), hasSelected);
			text += '<div id="' + elementId + 'cmbPresets">'+ cmb;
			text += '<span onClick="document.getElementById(\'' + elementId + 'Slot\').style.display=\'block\';">&nbsp;<img src="conf2.png" alt="dspSlots" style="cursor:pointer" width="15px" hight="15px" title="Expert Mode" ></span>';
			text += '</div>';
			//text += '<section id="' + elementId + 'cmbCustomPresets"></section>';
			text += '<div><br/></div>';
			let cssDisplay = "block";
			if(hasSelected) cssDisplay = "none";
			text += '<div id="' + elementId + 'Slot" style="display:' + cssDisplay + '">';
			for(let x in seq.val.profile[prof]["seq"]){
				let name = elementId + fieldPref + x;
				text += '<input class="LPS" type="range" orient="vertical" min="0" max="8" step="1" name="' + name + '" value="' + seq.val.profile[prof]["seq"][x] + '" title=""  id="' + name + '" onChange="setFormChanged(true);" >' + "\n";
			}
			text += '</div>';
		} else if(seite == 'pwr') {
			let name = elementId + fieldPref;
			text += '<label class="textstyle4" for="' + name + '">max. Leistung: </label>';
			text += '<input class="LPS" type="number" min="0" max="100" step="1" name="' + name + '" value="' + seq.val.profile[prof].pwr + '" title="" id="' + name + '" onChange="setFormChanged(true);" >';
			text += '<span class="textstyle4"> %<br/></span>';
		}
		text += '</div>';
	}

	text += '<div style="clear:both"></div>';
	text += '</div>';
	text += '</div>';
	text += '<div style="clear:both"></div>';
	text += '</div>';
	text += '</div>';
	text += '</div>';

	return text;
}

/**
 * Preset auf LightPort übertragen (onChange Event der Combobox)
 * 
 * @param {*} elemId 
 * @param {*} obj 
 */
function doPreset(elemId, obj) {
	let valArr = obj.value.split(",");
	for(let x=0; x<valArr.length; x++){
		if(x < gSlotCnt){
			document.getElementById(elemId+x).value = valArr[x].trim();
		}
	}
	//obj.value = "";
	setFormChanged(true);
}

/**
 * Übersichtstabelle der Presets erstellen
 * 
 * @param {*} file        - Json PresetDatei
 * @param {*} elementById - Platzhalter Element in das geschrieben wird
 */
function crtPresetTable(file, elementById){
	let xmlhttp = new XMLHttpRequest();
	xmlhttp.onload = function() {
		const myObj = JSON.parse(this.responseText);

		// EXTRACT VALUE FOR HTML HEADER. 
		// ('Book ID', 'Book Name', 'Category' and 'Price')
		let col = [['name', 'Name'], ['description', 'Beschreibung']];
		// CREATE DYNAMIC TABLE.
		let table = document.createElement("table");
		// CREATE HTML TABLE HEADER ROW USING THE EXTRACTED HEADERS ABOVE.
		let tr = table.insertRow(-1);                   // TABLE ROW.
		for (let i = 0; i < col.length; i++) {
			let th = document.createElement("th");      // TABLE HEADER.
			th.innerHTML = encodeHtml(col[i][1]).replaceAll(' ', "&nbsp;");
			tr.appendChild(th);
		}
		// ADD JSON DATA TO THE TABLE AS ROWS.
		for (let x in myObj["presets"]) {
			tr = table.insertRow(-1);
			for (let j = 0; j < col.length; j++) {
				let tabCell = tr.insertCell(-1);
				if(j==0)
					tabCell.innerHTML = encodeHtml(myObj["presets"][x][col[j][0]]).replaceAll(' ', "&nbsp;");
				else
					tabCell.innerHTML = encodeHtml(myObj["presets"][x][col[j][0]]);
			}
		}
		// FINALLY ADD THE NEWLY CREATED TABLE WITH JSON DATA TO A CONTAINER.
		elementById.innerHTML = "";
		elementById.appendChild(table);


	}
	xmlhttp.open("POST", file, false); // Synchronous
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();
}

/**
 * Form senden (JSON)
 * 
 * Parameter:
 * @param {*} mode  - 'save' | 'prev' | ...
 * @param {*} seite - 'ports' | ...
 */
/*
function formSend(mode, seite){

	let url = "";
	let fieldPref = "";
	
	if(mode == 'save')
		url = "/save";
	else if(mode == 'prev')
		url = "/prev";
	
	if(seite == 'ports') {
		url += "LightSequence";
		fieldPref = "_S";
	} else if(seite == 'pwr') {
		url += "LightPower";
		fieldPref = "_P";
	}


	let form = document.forms["form_" + seite];
	// Felder auslesen und JSON erstellen
	let obj = {};
	let elementName;
	if(seite == 'ports') {
		for(let x=1; x<=gPortCnt; x++){
			for(let y=0;y<gSlotCnt;y++){
				elementName = "LP"+ x + fieldPref + y;
				obj[elementName] = form[elementName].value ;
			}
		}
	} else if(seite == 'pwr') {
		for(let x=1; x<=gPortCnt; x++){
			elementName = "LP"+ x + fieldPref;
			obj[elementName] = form[elementName].value ;
		}
	}
	let json = JSON.stringify(obj);

	let xmlhttp = new XMLHttpRequest();
	xmlhttp.open("POST", url);
	xmlhttp.setRequestHeader("Content-type", "application/json");
	xmlhttp.send(json);
	if(mode == 'save')
		location.reload(true);
}
*/

/**
 * WebRequest zum Reset des ESP
 */
function resetEsp(){
	alert("Info: AP wird nur bei gedrückter Taste wieder eröffnet");
	let xmlhttp = new XMLHttpRequest();
	xmlhttp.open("POST", "/reset");
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send();
	//alert("Hardware wird neu gestartet.\nWLAN prüfen und Webseite neu laden (F5).");
	//location.reload(true);
	location.href = "/";
}

/**
 * Datei downloaden
 * 
 * @param {*} file 
 */
function download(file) {
	let element = document.createElement('a');
	element.setAttribute('href', file);
	element.setAttribute('download', "download");

	document.body.appendChild(element);
	element.click();
	document.body.removeChild(element);
}

/**
 * Seiten Titel setzen
 * 
 * @param {*} titel 
 */
function setTitel(titel){
	//document.title = gConfigObj["ProductName"] + " - " + titel + " (" + gConfigObj["ModelName"] + ")";
	document.title = gConfigObj["ModelName"] + " - " + titel + " (" + gConfigObj["ProductName"] + ")";
}

/**
 * Sonderzeichen in HTML Code umsetzen
 * @param {*} text 
 * @returns 
 */
function encodeHtml(text){
	/*
	let encodedStr = text.replace(/[\u00A0-\u9999<>\&]/g, function(i) {
		return '&#'+i.charCodeAt(0)+';';
	});
	return encodedStr;
	*/
	return String(text).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;').replace(/ä/g, '&auml;').replace(/Ä/g, '&Auml;').replace(/ö/g, '&ouml;').replace(/Ö/g, '&Ouml;').replace(/ü/g, '&uuml;').replace(/Ü/g, '&Uuml;');
}

/**
 * SSID erstellen -> Sonderzeichen durch '_' ersetzen und Leerzeichen entfernen
 * @param {*} ssid 
 * @returns 
 */
/*
function convert2ssid(ssid){
	return ssid.replace(/[\u00A0-\u9999<>\&]/g, '_').replaceAll(' ', '');
}
*/

/**
 * Config CH1 Tabelle erstellen und in HTML schreiben
 * - HTML in ID "table_ch1" schreiben
 * 
 */
/*
function crtConfigCh1Table(){
	let table = '<table id="table_4bd8ccd3" cellpadding="3" cellspacing="1" >';
	table += '<tr>';
	table += '<td width="16%" height="20px" ></td>';
	
	for(let x = 1; x <= gPortCnt; x++){
		table += '<td width="16%" height="20px" style="vertical-align: top; overflow:hidden; ">';
		table += '<div class="cell_ch">';
		table += '<div class="textstyle6">';
		table += '<span class="textstyle4">' + gSequenceObj["ports"][x-1]["name"] + '<br/><span class="w3-small">(Port ' + x + ')</span></span>';
		table += '</div>';
		table += '</div>';
		table += '</td>';
	}
	
	table += '</tr>';
	
	for(let x = 0; x < 3; x++) {	// Schalterpositionen
		table += '<tr>';
		for(let y = 0; y <= gPortCnt; y++) {	// Index 0 ist die Positionsbeschreibung
			table += '<td width="16%" height="20px" style="vertical-align: top; overflow:hidden; ">';
			table += '<div class="cell_ch">';
			if(y==0) {
				table += '<div class="textstyle1">';
				table += '<span class="textstyle4">';
				switch(x) {
					case 0:
						table += "low";
						break;
					case 1:
						table += "middle";
						break;
					case 2:
						table += "high";
						break;
				}
				table += '</span>';
				table += '</div>';
			} else {
				table += '<div class="textstyle6">';
				let chkBox = "chk1_" + x.toString() + y.toString();
				let isChecked = 'checked="checked"';//gSequenceObj["ports"][y-1]["swpos"][x] ? 'checked="checked"' : '';
				table += '<input name="' + chkBox + '" type="checkbox" value="' + chkBox + '" title="" ' + isChecked + '  id="' + chkBox + '" onChange="setFormChanged(true);">';
				table += '</div>';
			}
			table += '</div>';
			table += '</td>';
		}
		table += '</tr>';
	}
	table += '</table>';
	
	document.getElementById("table_ch1").innerHTML = table;
}
*/

function crtConfigCh1ComboBoot(){
	let cmbBox = '<select name="cmbCh1Pos" title="Simmulierte Schalterposition beim Start"  id="cmbCh1Pos" onChange="setFormChanged(true);">';
	for(let x = 0; x < 3; x++) {
		let text = "";
		switch(x) {
			case 0:
				text = "low";
				break;
			case 1:
				text = "middle";
				break;
			case 2:
				text = "high";
				break;
		}
		let isBoot = gSequenceObj["ch1Boot"]==x ? 'selected="selected"' : '';
		cmbBox += '<option value="'+x+'" '+isBoot+'>'+text+'</option>';
	}
	cmbBox += '</select>';

	document.getElementById("combo_ch1").innerHTML = cmbBox;
}

function crtConfigCh2ComboBoot(){
	let cmbBox = '<select name="cmbCh2Pos" title="Simmulierte Schalterposition beim Start"  id="cmbCh2Pos" onChange="setFormChanged(true);">';
	for(let x = 0; x < 3; x++) {
		let text = "";
		switch(x) {
			case 0:
				text = "low";
				break;
			case 1:
				text = "high";
				break;
			case 2:
				text = "Taster";
				break;
			}
		let isBoot = gSequenceObj["ch2Boot"]==x ? 'selected="selected"' : '';
		cmbBox += '<option value="'+x+'" '+isBoot+'>'+text+'</option>';
	}
	cmbBox += '</select>';

	document.getElementById("combo_ch2").innerHTML = cmbBox;
}
/*
function crtConfigPortBezeichnung(){
	let bez = "";
	for(let x = 1; x <= gPortCnt; x++) {
		let val = gSequenceObj["ports"][x-1]["name"];
		let name = "LPname_"+x.toString();
		bez += '<span class="textstyle4">Port ' + x.toString() + ' </span>';
		bez += '<input class="LPNAME" type="text" value="'+encodeHtml(val)+'" title="" name="'+name+'" maxlength="25"  id="'+name+'" onChange="setFormChanged(true);">';
		bez += '<span class="textstyle4"><br/></span>';
	}
	document.getElementById("input_bez").innerHTML = bez;
}
*/

function crtConfigConfigValues(){
	document.getElementById("ModelName").value = gConfigObj["ModelName"];
	document.getElementById("AP_SSID").value = gConfigObj["AP"]["ssid"];
	document.getElementById("AP_PWD").value = gConfigObj["AP"]["pwd"];
	document.getElementById("AP_IP").value = gConfigObj["AP"]["ip"];
}

/**
 * Form speichern
 * 
 * @param {*} url - URL die an den Server gesendet wird
 * @param {*} mode - an FormData wird dieses Feld als mode=xxx angehängt
 * @param {*} form - Objekt der HTML Form
 * @param {*} bOkBox - bei Response 2xx(ok) MsgBox anzeigen
 * @param {*} callbackStatus - weglassen, da nur Defaultfunktion möglich
 */
function saveForm(url, mode, form, bOkBox=true, callbackStatus=displayMsgBox){
	let xhr = new XMLHttpRequest();
	xhr.onreadystatechange = function() {
		if (xhr.readyState != 4) {
			return;
		}
		// Server-Antwort ist eingetroffen!
		if (this.status >= 200 && this.status <= 299) {
			// Server-Antwort in Ordnung
			if(bOkBox){
				if(this.responseText.length > 0) {
					callbackStatus(this.responseText);
				}else {
					callbackStatus("Erfolgreich gespeichert.");
				}
			}
		} else {
			callbackStatus("Es ist ein Fehler aufgetreten:\n" + this.status);
		}
	}

	xhr.onload = function() {
		//alert(this.response);
	};

	xhr.upload.onprogress = function (ev) {
		if(bOkBox){
			callbackStatus(String(ev.loaded/ev.total*100) + " %");
		}
	};

	let fd = new FormData(form);
	if(mode != ""){
		fd.append('mode', mode);
	}

	xhr.open("POST", url, true);
	xhr.send(fd);
}


/**
 * HTML Code für die MsgBox erstellen
 * - HTML in ID "MsgBox" schreiben
 * 
 * @param {*} title - Text der im Titel stehen soll. ansonsten kein Titel
 */
function crtMsgBox(title=""){
	let clickClose = ' onclick="document.getElementById(\'idMsgBox\').style.display=\'none\';" ';
	let html = '<div id="idMsgBox" class="w3-modal">';
	html += '<div class="w3-modal-content"' + clickClose + '>';
	if(title != ""){
		html += '<div class="w3-container vk_light_grey">';
		html += '<span' + clickClose + ' class="w3-button w3-display-topright">&times;</span>';
		html += '<p id="idMagBoxTitle">' + title + '</p>';
		html += '</div>';
	}
	html += '<div class="w3-container">';
	if(title == "")
		html += '<span' + clickClose + ' class="w3-button w3-display-topright">&times;</span>';
	html += '<p id="idMsgBoxText">...</p>';
	html += '</div>';
	html += '</div>';
	html += '</div>';
	document.getElementById('MsgBox').innerHTML = html;
}
/**
 * Text in MsgBox schreiben und diese anzeigen
 * 
 * @param {*} text - Nachrichtentext
 * @param {*} title - Titeltext
 */
function displayMsgBox(text, title=null){
	if(document.getElementById('idMagBoxTitle') != null && title != null){
		document.getElementById('idMagBoxTitle').innerHTML = title;
	}
	document.getElementById('idMsgBoxText').innerHTML = text;
	document.getElementById('idMsgBox').style.display='block';
}

/**
 * Beim verlassen der Seite prüfen ob die <form> gespeichert wurde
 */
function checkFormOnLeave(){
	window.onbeforeunload = function() {
		if(!gFormPrev) {
			if (gFormChanged && !gFormSubmitted) {
				return 'Alle nicht gespeicherten Daten gehen verloren.\nMöchten Sie die Seite wirklich verlassen?';
			}
		} else {
			gFormPrev = false;
		}
	};
}
function setFormChanged(val){
	gFormChanged = val;
	setFormSubmitted(false);
}
function setFormSubmitted(val){
	gFormSubmitted=val;
}
function setFormPreview(val){
	gFormPrev=val;
}

function checkFormOnLeaveConfig_html(){
	window.onbeforeunload = function() {
		if ((gFormChanged && !gFormSubmitted) || (gFormChangedConf && !gFormSubmittedConf)) {
			return 'Alle nicht gespeicherten Daten gehen verloren.\nMöchten Sie die Seite wirklich verlassen?';
		}
	};
}
function setFormChangedConf(val){
	gFormChangedConf = val;
	setFormSubmittedConf(false);
}
function setFormSubmittedConf(val){
	gFormSubmittedConf=val;
}
