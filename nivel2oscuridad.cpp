#include "nivel2oscuridad.h"

Nivel2Oscuridad::Nivel2Oscuridad(QGraphicsView* view, QObject* parent)
    : QObject(parent)
    , m_scene(new QGraphicsScene(this)), m_view(view)
    , m_fondo(nullptr), m_barcoItem(nullptr)
    , m_bx(600.0f), m_by(2400.0f)
    , m_scrollY(0.0f)
    , m_krakenItem(nullptr)
    , m_kFrame(0), m_kFrameW(0), m_kFrameH(0)
    , m_kxBase(980.0f), m_kyBase(2280.0f)
    , m_kTiempo(0.0f), m_kAmplitud(55.0f), m_kFrecuencia(1.1f)
    , m_agresividad(AGR_INICIAL)
    , m_tAtaque(0.0f), m_intervaloAtaque(INTERVALO_BASE)
    , m_evasionesArriba(0), m_evasionesAbajo(0)
    , m_evasionesIzq(0),    m_evasionsDer(0)
    , m_byAnt(2400.0f), m_bxAnt(600.0f)
    , m_overlayItem(nullptr)
    , m_hudSalud(nullptr), m_hudPuntos(nullptr), m_hudTiempo(nullptr)
    , m_salud(SALUD_MAX), m_puntuacion(0), m_tiempoJuego(0.0f), m_activo(false)
    , m_wPress(false), m_sPress(false), m_aPress(false), m_dPress(false)
{
    m_scene->setSceneRect(0, 0, 1280, 2560);
    m_timerLoop  = new QTimer(this);
    m_timerKAnim = new QTimer(this);
    m_timerSpawn = new QTimer(this);
    connect(m_timerLoop,  &QTimer::timeout, this, &Nivel2Oscuridad::tick);
    connect(m_timerKAnim, &QTimer::timeout, this, &Nivel2Oscuridad::animarKraken);
    connect(m_timerSpawn, &QTimer::timeout, this, &Nivel2Oscuridad::spawnObstaculo);
}

Nivel2Oscuridad::~Nivel2Oscuridad() { pausar(); }

void Nivel2Oscuridad::iniciar()
{
    m_scene->clear();
    m_objetos.clear();
    m_salud = SALUD_MAX; m_puntuacion = 0; m_tiempoJuego = 0.0f; m_activo = true;
    m_kTiempo = 0.0f; m_tAtaque = 0.0f; m_agresividad = AGR_INICIAL;
    m_intervaloAtaque = INTERVALO_BASE;
    m_evasionesArriba = m_evasionesAbajo = m_evasionesIzq = m_evasionsDer = 0;
    m_bx = 600.0f; m_by = 2400.0f;
    m_scrollY = 0.0f;
    m_kxBase = 980.0f; m_kyBase = 2280.0f;
    m_overlayItem = nullptr;
    m_hudSalud = nullptr; m_hudPuntos = nullptr; m_hudTiempo = nullptr;

    setupFondo();
    setupBarco();
    setupKraken();
    setupLinterna();
    setupHUD();

    m_view->setScene(m_scene);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->centerOn(m_bx, m_by);

    m_timerLoop->start(16);
    m_timerKAnim->start(130);
    m_timerSpawn->start(2200);
}

void Nivel2Oscuridad::pausar() {
    m_timerLoop->stop();
    m_timerKAnim->stop();
    m_timerSpawn->stop();
}

void Nivel2Oscuridad::setupFondo()
{
    QPixmap px(":/imagenes2/imagenes/Fondo_nivel2.png");
    if (px.isNull()) { px = QPixmap(1280, 2560); px.fill(QColor(5, 8, 25)); }
    px = px.scaled(1280, 2560, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Oscurecer el fondo directamente
    QPainter p(&px);
    p.fillRect(px.rect(), QColor(0, 0, 20, 180));
    p.end();

    m_fondo = new QGraphicsPixmapItem(px);
    m_fondo->setPos(0, 0);
    m_fondo->setZValue(0);
    m_scene->setSceneRect(0, 0, 1280, 2560);
    m_scene->addItem(m_fondo);
}

void Nivel2Oscuridad::setupBarco()
{
    QPixmap px(":/imagenes2/imagenes/barco_cenital.png");
    if (px.isNull()) { px = QPixmap(80,80); px.fill(QColor(139,90,43)); }
    QPixmap sf = quitarFondo(px, Qt::black, 50);
    sf = sf.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_barcoItem = new QGraphicsPixmapItem(sf);
    m_barcoItem->setPos(m_bx, m_by);
    m_barcoItem->setZValue(3);
    m_scene->addItem(m_barcoItem);
}

void Nivel2Oscuridad::setupKraken()
{
    m_krakenSheet = QPixmap(":/imagenes2/imagenes/kraken.png");
    if (m_krakenSheet.isNull()) {
        m_krakenSheet = QPixmap(400, 480);
        m_krakenSheet.fill(QColor(180,20,20));
    }
    m_kFrameW = m_krakenSheet.width()  / 5;
    m_kFrameH = m_krakenSheet.height() / 6;
    QPixmap frame = quitarFondo(
        m_krakenSheet.copy(0, 0, m_kFrameW, m_kFrameH),
        QColor(0,128,128), 55);
    frame = frame.scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_krakenItem = new QGraphicsPixmapItem(frame);
    m_krakenItem->setPos(m_kxBase, m_kyBase);
    m_krakenItem->setZValue(4);
    m_scene->addItem(m_krakenItem);
}

void Nivel2Oscuridad::setupLinterna()
{
    // Sin overlay
}

void Nivel2Oscuridad::reconstruirLinterna()
{
    // Sin reconstrucción
}

void Nivel2Oscuridad::setupHUD()
{
    QFont f("Arial", 14, QFont::Bold);

    m_hudSalud = new QGraphicsTextItem();
    m_hudSalud->setDefaultTextColor(QColor(255, 80, 80));
    m_hudSalud->setFont(f);
    m_hudSalud->setZValue(50);
    m_scene->addItem(m_hudSalud);

    m_hudPuntos = new QGraphicsTextItem();
    m_hudPuntos->setDefaultTextColor(QColor(255, 215, 0));
    m_hudPuntos->setFont(f);
    m_hudPuntos->setZValue(50);
    m_scene->addItem(m_hudPuntos);

    m_hudTiempo = new QGraphicsTextItem();
    m_hudTiempo->setDefaultTextColor(Qt::white);
    m_hudTiempo->setFont(f);
    m_hudTiempo->setZValue(50);
    m_scene->addItem(m_hudTiempo);

    refrescarHUD();
}

void Nivel2Oscuridad::tick()
{
    if (!m_activo) return;
    m_tiempoJuego += DT;
    m_puntuacion = static_cast<int>(m_scrollY);

    moverBarco();
    scrollFondo();
    actualizarKraken();
    actualizarObjetos();
    detectarColisiones();
    refrescarHUD();

    if (m_scrollY >= 2300.0f) {
        m_activo = false; pausar();
        emit nivelCompletado();
    }
}

void Nivel2Oscuridad::scrollFondo()
{
    m_scrollY += 40.0f * DT;
    m_by = 2400.0f - m_scrollY;
    m_view->centerOn(m_bx, m_by);
    m_kyBase = m_by - 120.0f;
}

void Nivel2Oscuridad::moverBarco()
{
    float vx = 0;
    if (m_aPress) vx = -VEL_BARCO;
    if (m_dPress) vx =  VEL_BARCO;
    m_bx = qBound(10.0f, m_bx + vx * DT, 1190.0f);
    if (m_barcoItem) m_barcoItem->setPos(m_bx, m_by);
}

void Nivel2Oscuridad::actualizarKraken()
{
    if (!m_krakenItem) return;
    m_kTiempo += DT; m_tAtaque += DT;

    // Movimiento sinusoidal en Y
    float ky = m_kyBase + m_kAmplitud *
                              static_cast<float>(qSin(m_kFrecuencia * m_kTiempo));

    // Movimiento suave en X hacia su objetivo
    float kxActual = static_cast<float>(m_krakenItem->pos().x());
    float kxNuevo  = kxActual + (m_kxBase - kxActual) * 0.02f;

    m_krakenItem->setPos(kxNuevo, ky);
    if (m_tAtaque >= m_intervaloAtaque) { razonarKraken(); m_tAtaque = 0.0f; }
}

void Nivel2Oscuridad::razonarKraken()
{
    if (m_bx < m_bxAnt - 2.0f)      m_evasionesIzq++;
    else if (m_bx > m_bxAnt + 2.0f) m_evasionsDer++;
    m_byAnt = m_by; m_bxAnt = m_bx;

    int totalV = m_evasionesArriba + m_evasionesAbajo;
    float probArriba = totalV > 0
                           ? static_cast<float>(m_evasionesArriba) / totalV : 0.5f;
    float ruido = (1.0f - m_agresividad) * 0.35f;
    probArriba += ruido * (static_cast<float>(
                               QRandomGenerator::global()->generateDouble()) - 0.5f);

    // Se mueve en X por toda la pantalla cerca del barco
    m_kxBase = qBound(100.0f,
                      m_bx + 100.0f + static_cast<float>(
                          QRandomGenerator::global()->bounded(200)) - 100.0f,
                      1150.0f);
    // Se mueve en Y cerca del barco según historial
    float targetY = (probArriba > 0.5f) ? m_by - 100.0f : m_by + 100.0f;
    m_kyBase = qBound(m_by - 250.0f, targetY, m_by + 250.0f);

    m_kAmplitud   = 55.0f + m_agresividad * 90.0f;
    m_kFrecuencia = 1.1f  + m_agresividad * 2.0f;
    m_agresividad     = qMin(1.0f, m_agresividad + 0.035f);
    m_intervaloAtaque = INTERVALO_BASE * (1.0f - m_agresividad * 0.5f);
}

void Nivel2Oscuridad::animarKraken()
{
    if (!m_krakenItem || m_krakenSheet.isNull()) return;
    m_kFrame = (m_kFrame + 1) % 4;
    QPixmap frame = quitarFondo(
        m_krakenSheet.copy(m_kFrame * m_kFrameW, 0, m_kFrameW, m_kFrameH),
        QColor(0,128,128), 55);
    frame = frame.scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_krakenItem->setPixmap(frame);
}

void Nivel2Oscuridad::spawnObstaculo()
{
    if (!m_activo) return;
    if (m_pixBarril.isNull()) {
        m_pixBarril = QPixmap(":/imagenes2/imagenes/barril.png");
        if (m_pixBarril.isNull()) {
            m_pixBarril = QPixmap(44,44);
            m_pixBarril.fill(QColor(139,69,19));
        }
        m_pixBarril = m_pixBarril.scaled(44, 44,
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    for (int i = 0; i < 2; ++i) {
        Objeto obj;
        obj.item = new QGraphicsPixmapItem(m_pixBarril);
        obj.x0 = 50.0f + QRandomGenerator::global()->bounded(1180);
        obj.y0 = m_by - 400.0f - QRandomGenerator::global()->bounded(200);
        obj.vx = static_cast<float>(QRandomGenerator::global()->bounded(80)) - 40.0f;
        obj.vy = 40.0f + QRandomGenerator::global()->bounded(60);
        obj.t  = 0.0f; obj.activo = true;
        obj.item->setPos(obj.x0, obj.y0);
        obj.item->setZValue(5);
        m_scene->addItem(obj.item);
        m_objetos.append(obj);
    }
}

void Nivel2Oscuridad::actualizarObjetos()
{
    for (auto& obj : m_objetos) {
        if (!obj.activo) continue;
        obj.t += DT;
        obj.item->setPos(obj.x0 + obj.vx * obj.t,
                         obj.y0 + obj.vy * obj.t + 0.5f * GRAVEDAD * obj.t * obj.t);
        if (obj.item->pos().y() > m_by + 500.0f ||
            obj.item->pos().y() < m_by - 800.0f) {
            obj.activo = false;
            m_scene->removeItem(obj.item);
            delete obj.item; obj.item = nullptr;
        }
    }
    m_objetos.erase(
        std::remove_if(m_objetos.begin(), m_objetos.end(),
                       [](const Objeto& o){ return !o.activo; }),
        m_objetos.end());
}

void Nivel2Oscuridad::detectarColisiones()
{
    if (!m_barcoItem) return;
    QRectF bRect = m_barcoItem->sceneBoundingRect().adjusted(8,8,-8,-8);
    for (auto& obj : m_objetos) {
        if (!obj.activo || !obj.item) continue;
        if (bRect.intersects(obj.item->sceneBoundingRect())) {
            m_salud -= DANIO_OBJ;
            obj.activo = false;
            m_scene->removeItem(obj.item);
            delete obj.item; obj.item = nullptr;
        }
    }
    if (m_krakenItem)
        if (bRect.intersects(
                m_krakenItem->sceneBoundingRect().adjusted(15,15,-15,-15)))
            m_salud -= DANIO_KRAKEN;
    m_salud = qMax(0, m_salud);
    if (m_salud <= 0) { m_activo = false; pausar(); emit nivelFallido(m_puntuacion); }
}

void Nivel2Oscuridad::refrescarHUD()
{
    // Obtener la esquina superior izquierda real de la cámara
    QPointF topLeft = m_view->mapToScene(0, 0);

    if (m_hudSalud) {
        m_hudSalud->setPos(topLeft.x() + 10, topLeft.y() + 10);
        m_hudSalud->setPlainText(QString("❤  %1 / %2").arg(m_salud).arg(SALUD_MAX));
    }
    if (m_hudPuntos) {
        m_hudPuntos->setPos(topLeft.x() + 10, topLeft.y() + 38);
        m_hudPuntos->setPlainText(QString("⭐  %1 pts").arg(m_puntuacion));
    }
    if (m_hudTiempo) {
        m_hudTiempo->setPos(topLeft.x() + 10, topLeft.y() + 66);
        m_hudTiempo->setPlainText(QString("⏱  %1 s").arg(static_cast<int>(m_tiempoJuego)));
    }
    emit hudActualizado(m_salud, m_puntuacion, static_cast<int>(m_tiempoJuego));
}

void Nivel2Oscuridad::keyPressEvent(QKeyEvent* e) {
    switch(e->key()) {
    case Qt::Key_W: case Qt::Key_Up:    m_wPress=true; break;
    case Qt::Key_S: case Qt::Key_Down:  m_sPress=true; break;
    case Qt::Key_A: case Qt::Key_Left:  m_aPress=true; break;
    case Qt::Key_D: case Qt::Key_Right: m_dPress=true; break;
    default: break;
    }
}

void Nivel2Oscuridad::keyReleaseEvent(QKeyEvent* e) {
    switch(e->key()) {
    case Qt::Key_W: case Qt::Key_Up:    m_wPress=false; break;
    case Qt::Key_S: case Qt::Key_Down:  m_sPress=false; break;
    case Qt::Key_A: case Qt::Key_Left:  m_aPress=false; break;
    case Qt::Key_D: case Qt::Key_Right: m_dPress=false; break;
    default: break;
    }
}

QPixmap Nivel2Oscuridad::quitarFondo(const QPixmap& src, const QColor& bg, int tol)
{
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    int r0=bg.red(), g0=bg.green(), b0=bg.blue();
    for (int y=0; y<img.height(); ++y)
        for (int x=0; x<img.width(); ++x) {
            QColor c = img.pixelColor(x,y);
            if (qAbs(c.red()-r0)<tol && qAbs(c.green()-g0)<tol && qAbs(c.blue()-b0)<tol)
                img.setPixelColor(x, y, Qt::transparent);
        }
    return QPixmap::fromImage(img);
}
