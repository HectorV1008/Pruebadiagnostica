#!/usr/bin/env node
// polinomio.js
// Dado n >= 0:
// a) genera coeficientes de (x+1)^n usando triángulo de Pascal (fórmula multiplicativa)
// b) muestra el polinomio y muestra por pasos el cálculo f(x) = (x+1)^n según el polinomio generado
// Usa BigInt para enteros grandes y mide tiempos. Si n==100 escribirá resultados en resultados_n100.txt
// powershell -Command "node 'c:\Users\Hector\Desktop\PruebaD\problema 2\polinomio.js' 100 2"
const fs = require('fs');

function now() { return process.hrtime.bigint(); }
function diffMs(t1, t2) { return Number(t2 - t1) / 1e6; }

function generateCoefficients(n) {
  // Genera los coeficientes C(n,0)..C(n,n) en O(n) usando la relación multiplicativa
  const coeffs = [];
  let c = 1n;
  coeffs.push(c);
  for (let k = 1; k <= n; ++k) {
    // c = c * (n - k + 1) / k
    c = (c * BigInt(n - k + 1)) / BigInt(k);
    coeffs.push(c);
  }
  return coeffs;
}

function polynomialToString(coeffs) {
  // Construye una representación del polinomio P(x) = sum coeff[k] * x^k
  const parts = [];
  const n = coeffs.length - 1;
  for (let k = 0; k <= n; ++k) {
    const a = coeffs[k].toString();
    if (k === 0) parts.push(`${a}`);
    else if (k === 1) parts.push(`${a}*x`);
    else parts.push(`${a}*x^${k}`);
  }
  return parts.join(' + ');
}

function evaluatePolynomialSteps(coeffs, xVal) {
  const x = BigInt(xVal);
  const n = coeffs.length - 1;
  const steps = [];
  let xPow = 1n; // x^0
  let sum = 0n;
  for (let k = 0; k <= n; ++k) {
    // term = coeffs[k] * x^k
    if (k === 0) xPow = 1n;
    else xPow *= x;
    const term = coeffs[k] * xPow;
    sum += term;
    steps.push({k, coeff: coeffs[k], xPow, term, sum});
  }
  return {result: sum, steps};
}

function usage() {
  console.log('Uso: node polinomio.js <n> <x> [--out file]');
  console.log('Ejemplo: node polinomio.js 5 2 --out resultados.txt');
}

function main() {
  const argv = process.argv.slice(2);
  if (argv.length < 2) { usage(); process.exit(1); }
  const n = Number(argv[0]);
  const x = argv[1];
  let outFile = null;
  for (let i = 2; i < argv.length; ++i) {
    if (argv[i] === '--out' && i + 1 < argv.length) { outFile = argv[i+1]; i++; }
  }
  if (!Number.isInteger(n) || n < 0) { console.error('n debe ser entero no negativo'); process.exit(2); }

  const log = [];

  const t0 = now();
  const coeffs = generateCoefficients(n);
  const t1 = now();
  const genMs = diffMs(t0, t1);
  log.push(`Generación de coeficientes: ${genMs.toFixed(3)} ms`);

  // Mostrar polinomio
  const t2 = now();
  const polyStr = polynomialToString(coeffs);
  const t3 = now();
  const polyMs = diffMs(t2, t3);
  log.push(`Construcción cadena polinomio: ${polyMs.toFixed(3)} ms`);

  // Evaluación por pasos
  const t4 = now();
  const evalRes = evaluatePolynomialSteps(coeffs, x);
  const t5 = now();
  const evalMs = diffMs(t4, t5);
  log.push(`Evaluación por pasos: ${evalMs.toFixed(3)} ms`);

  // Resultado final
  const resultStr = evalRes.result.toString();

  // Preparar salida textual
  let out = '';
  out += `Polinomio (x+1)^${n} como suma de coeficientes * x^k:\n`;
  out += polyStr + '\n\n';
  out += `Evaluación para x = ${x}:\n`;
  for (const s of evalRes.steps) {
    out += `k=${s.k}: coeff=${s.coeff.toString()} * x^${s.k}(${s.xPow.toString()}) = ${s.term.toString()}  => suma=${s.sum.toString()}\n`;
  }
  out += `\nResultado final f(${x}) = ${resultStr}\n\n`;
  out += 'Tiempos (ms):\n';
  for (const l of log) out += l + '\n';

  // Mostrar por stdout
  console.log(out);

  // Si n==100 o si se pidió --out, escribir fichero de resultados.
  // Por defecto, escribir en la misma carpeta donde está este script (no en el cwd).
  if (n === 100 || outFile) {
    const path = require('path');
    const defaultName = 'resultados_n100js.txt';
    // __dirname es la carpeta del script; si se pasó --out, usarla; si no, escribir junto al script
    const dest = outFile ? outFile : path.join(__dirname, defaultName);
    try {
      fs.writeFileSync(dest, out, { encoding: 'utf8' });
      console.log(`Escrito archivo de resultados: ${dest}`);
    } catch (e) {
      console.error('No se pudo escribir el fichero de resultados:', e.message);
    }
  }
}

if (require.main === module) main();
