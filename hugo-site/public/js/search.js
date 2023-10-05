//
// code taken from: https://www.mattwalters.net/posts/hugo-and-lunr/
//

var idx = null;         // Lunr index
var resultDetails = []; // Will hold the data for the search results (titles and summaries)
var searchResults;      // The element on the page holding search results
var searchInput;        // The search box element

window.onload = function () {
  var request = new XMLHttpRequest();

  searchResults = document.getElementById('content-body');
  searchInput = document.getElementById('search-input');

  request.overrideMimeType("application/json");
  request.open("GET", "/index.json", true); // Request the JSON file created during build
  request.onload = function () {
    if (request.status >= 200 && request.status < 400) {
      // Success response received in requesting the index.json file
      var documents = JSON.parse(request.responseText);

      // Build the index so Lunr can search it.  The `ref` field will hold the URL
      // to the page/post.  title, summary, and body will be fields searched.
      idx = lunr(function () {
        this.ref('ref');
        this.field('title');
        this.field('summary');
        this.field('body');

        // Loop through all the items in the JSON file and add them to the index
        // so they can be searched.
        documents.forEach(function (doc) {
          this.add(doc);
          resultDetails[doc.ref] = {
            'title': doc.title,
            'summary': doc.summary,
          };
        }, this);
      });
    } else {
      searchResults.innerHTML = 'Error loading search results';
    }
  };

  request.onerror = function () {
    searchResults.innerHTML = 'Error loading search results';
  };

  request.send();
};

function do_search() {
  var query = $(searchInput).val();
  var results = idx.search(query);
  renderSearchResults(results);

  if (searchInput.value == '') {
    searchResults.innerHTML = '';
  }
}

function renderSearchResults(results) {
  var dst = $(searchResults);
  dst.empty()

  if (results.length == 0) {
    dst.append('No results found');
    return;
  }

  var ul = $("<ul class='search'>");
  results.forEach(function(r) {
    var title = resultDetails[r.ref].title.trim() || "- <i>[document without title]</i>";

    $(ul).append(
      '<li>' +
      '  <a href="' + r.ref + '">' + title + '</a><br>' +
      '  <cite>' + r.ref + '</cite><br>' +
      '  <p>' + resultDetails[r.ref].summary + '</p>' +
      '</li>'
    );
  });

  dst.append(ul);
}
