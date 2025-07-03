const suffixTableBody = document.querySelector('#suffixTable tbody');
const prevBtn = document.getElementById('prevBtn');
const nextBtn = document.getElementById('nextBtn');
const pageInfo = document.getElementById('pageInfo');
const generateBtn = document.getElementById('generateBtn');
const textInput = document.getElementById('textInput');

let currentPage = 1;
const rowsPerPage = 15;
let data = [];

// Función para generar suffix array (simulación backend)
function generateSuffixArray(text) {
  const suffixes = [];
  for (let i = 0; i < text.length; i++) {
    suffixes.push({
      index: i,
      length: text.length - i,
      original_char: text[i],
      suffix: text.slice(i)
    });
  }
  // Ordena lexicográficamente el suffix array
  suffixes.sort((a, b) => a.suffix.localeCompare(b.suffix));
  // Asigna rank basado en el orden
  suffixes.forEach((item, idx) => (item.rank = idx));
  return suffixes;
}

// Renderiza la página actual
function renderTablePage(page) {
  suffixTableBody.innerHTML = '';

  const start = (page - 1) * rowsPerPage;
  const end = start + rowsPerPage;
  const pageData = data.slice(start, end);

  pageData.forEach(item => {
    const tr = document.createElement('tr');

    tr.innerHTML = `
      <td>${item.rank}</td>
      <td>${item.index}</td>
      <td>${item.original_char}</td>
      <td>${item.length}</td>
      <td>${item.suffix}</td>
    `;

    suffixTableBody.appendChild(tr);
  });

  pageInfo.textContent = `Página ${page} de ${Math.ceil(data.length / rowsPerPage)}`;

  prevBtn.disabled = page === 1;
  nextBtn.disabled = page === Math.ceil(data.length / rowsPerPage);
}

// Botones para cambiar página
prevBtn.addEventListener('click', () => {
  if (currentPage > 1) {
    currentPage--;
    renderTablePage(currentPage);
  }
});

nextBtn.addEventListener('click', () => {
  if (currentPage < Math.ceil(data.length / rowsPerPage)) {
    currentPage++;
    renderTablePage(currentPage);
  }
});

// Botón Generar
generateBtn.addEventListener('click', () => {
  const text = textInput.value.trim();
  if (!text) {
    alert('Por favor, escribe una palabra válida 😅');
    return;
  }
  data = generateSuffixArray(text);
  currentPage = 1;
  renderTablePage(currentPage);
});

// Al cargar la página, muestra una tabla vacía o con datos de ejemplo si quieres
