// Efectos de scroll suave
document.addEventListener('DOMContentLoaded', function() {
    // Animación de entrada para las tarjetas
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -50px 0px'
    };

    const observer = new IntersectionObserver(function(entries) {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, observerOptions);

    // Observar todas las tarjetas de componentes
    const cards = document.querySelectorAll('.component-card, .benefit-item, .workflow-step');
    cards.forEach(card => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        card.style.transition = 'opacity 0.6s ease, transform 0.6s ease';
        observer.observe(card);
    });

    // Efecto de hover mejorado para las tarjetas
    const componentCards = document.querySelectorAll('.component-card');
    componentCards.forEach(card => {
        card.addEventListener('mouseenter', function() {
            this.style.borderTopColor = '#3498db';
        });
        
        card.addEventListener('mouseleave', function() {
            this.style.borderTopColor = '#f39c12';
        });
    });

    // Agregar efecto de click a las tarjetas (opcional)
    componentCards.forEach(card => {
        card.addEventListener('click', function() {
            this.style.transform = 'scale(0.98)';
            setTimeout(() => {
                this.style.transform = '';
            }, 150);
        });
    });

    // Smooth scroll para navegación (si se agregan enlaces)
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function (e) {
            e.preventDefault();
            const target = document.querySelector(this.getAttribute('href'));
            if (target) {
                target.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });

    // Mostrar información adicional al hacer hover en los iconos
    const icons = document.querySelectorAll('.component-icon');
    icons.forEach(icon => {
        icon.addEventListener('mouseenter', function() {
            this.style.transform = 'scale(1.1) rotate(5deg)';
            this.style.transition = 'transform 0.3s ease';
        });
        
        icon.addEventListener('mouseleave', function() {
            this.style.transform = 'scale(1) rotate(0deg)';
        });
    });
});

// Función para agregar efecto de parallax sutil al header
window.addEventListener('scroll', function() {
    const header = document.querySelector('header');
    const scrolled = window.pageYOffset;
    if (header && scrolled < 300) {
        header.style.transform = `translateY(${scrolled * 0.5}px)`;
    }
});

// Agregar clase activa al hacer scroll
let lastScroll = 0;
window.addEventListener('scroll', function() {
    const currentScroll = window.pageYOffset;
    
    if (currentScroll > lastScroll && currentScroll > 100) {
        // Scrolling down
        document.body.classList.add('scrolled');
    } else {
        // Scrolling up
        document.body.classList.remove('scrolled');
    }
    
    lastScroll = currentScroll;
});
