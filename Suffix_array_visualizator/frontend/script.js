const fileInput = document.getElementById('fileInput');
const suffixTable = document.getElementById('suffixTable');
const tbody = suffixTable.querySelector('tbody');

fileInput.addEventListener('change', (event) => {
  const file = event.target.files[0];
  if (!file) return;

  const reader = new FileReader();
  reader.onload = function(e) {
    try {
      const data = JSON.parse(e.target.result);

      // Limpiar tabla
      tbody.innerHTML = '';

      data.forEach((item, i) => {
        const tr = document.createElement('tr');

        const tdRank = document.createElement('td');
        tdRank.textContent = i;

        const tdIndex = document.createElement('td');
        tdIndex.textContent = item.index;

        const tdChar = document.createElement('td');
        tdChar.textContent = item.suffix[0] || '';

        const tdLength = document.createElement('td');
        tdLength.textContent = item.suffix.length;

        const tdSuffix = document.createElement('td');
        tdSuffix.textContent = item.suffix;

        tr.appendChild(tdRank);
        tr.appendChild(tdIndex);
        tr.appendChild(tdChar);
        tr.appendChild(tdLength);
        tr.appendChild(tdSuffix);

        tbody.appendChild(tr);
      });

      suffixTable.style.display = 'table';
    } catch (error) {
      alert('Error al leer el archivo JSON: ' + error.message);
    }
  };

  reader.readAsText(file);
});
