/* ==========================================================================
   Inventory Management System — Showcase Site
   script.js
   ========================================================================== */

document.addEventListener('DOMContentLoaded', () => {
  setFooterYear();
  initNavToggle();
  initTerminalTyping();
  initScrollReveal();
});

/* ------------------------------------------------------------------------
   Footer year
   ------------------------------------------------------------------------ */
function setFooterYear() {
  const yearEl = document.getElementById('year');
  if (yearEl) {
    yearEl.textContent = new Date().getFullYear();
  }
}

/* ------------------------------------------------------------------------
   Mobile nav toggle
   ------------------------------------------------------------------------ */
function initNavToggle() {
  const toggle = document.getElementById('navToggle');
  const links = document.getElementById('navLinks');
  if (!toggle || !links) return;

  toggle.addEventListener('click', () => {
    const isOpen = links.classList.toggle('open');
    toggle.setAttribute('aria-expanded', String(isOpen));
    toggle.classList.toggle('is-active', isOpen);
  });

  // Close menu when a link is tapped
  links.querySelectorAll('a').forEach((link) => {
    link.addEventListener('click', () => {
      links.classList.remove('open');
      toggle.setAttribute('aria-expanded', 'false');
    });
  });
}

/* ------------------------------------------------------------------------
   Terminal typing animation
   Replays a shortened version of the real console menu so the hero
   mockup matches what the actual program prints on launch.
   ------------------------------------------------------------------------ */
function initTerminalTyping() {
  const target = document.getElementById('terminalBody');
  if (!target) return;

  const prefersReducedMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  const script = [
    '$ ./inventory',
    '[INFO] Loaded 10 product(s) from inventory.csv',
    '',
    '============================================================',
    '            INVENTORY MANAGEMENT SYSTEM',
    '============================================================',
    ' 1.  Add Product',
    ' 2.  Display Products',
    ' 3.  Search Product',
    ' 6.  Sell Product',
    ' 7.  Restock Product',
    '11.  Binary Search Product by ID',
    '12.  Low Stock Alert',
    '13.  Inventory Report',
    '15.  Exit',
    '============================================================',
    'Enter your choice: 13',
    '',
    '------------------------------------------------------------',
    'Total Products:            10',
    'Total Stock Units:         880',
    'Total Inventory Value:     $4219.30',
    'Most Expensive Product : Laptop ($999.99)',
    '------------------------------------------------------------',
  ].join('\n');

  if (prefersReducedMotion) {
    target.textContent = script;
    return;
  }

  let i = 0;
  const speed = 8; // ms per character

  function typeNext() {
    if (i <= script.length) {
      target.textContent = script.slice(0, i);
      target.innerHTML += '<span class="terminal-cursor"></span>';
      i += 2;
      window.requestAnimationFrame(() => setTimeout(typeNext, speed));
    } else {
      target.textContent = script;
      const cursor = document.createElement('span');
      cursor.className = 'terminal-cursor';
      target.appendChild(cursor);
    }
  }

  // Start after a short delay so the page settles first
  setTimeout(typeNext, 350);
}

/* ------------------------------------------------------------------------
   Scroll reveal
   Fades sections in as they enter the viewport.
   ------------------------------------------------------------------------ */
function initScrollReveal() {
  const prefersReducedMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;
  const sections = document.querySelectorAll('.section');

  if (prefersReducedMotion || !('IntersectionObserver' in window)) {
    sections.forEach((el) => el.classList.add('is-visible'));
    return;
  }

  sections.forEach((el) => {
    el.style.opacity = '0';
    el.style.transform = 'translateY(16px)';
    el.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
  });

  const observer = new IntersectionObserver(
    (entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          entry.target.style.opacity = '1';
          entry.target.style.transform = 'translateY(0)';
          observer.unobserve(entry.target);
        }
      });
    },
    { threshold: 0.12 }
  );

  sections.forEach((el) => observer.observe(el));
}
