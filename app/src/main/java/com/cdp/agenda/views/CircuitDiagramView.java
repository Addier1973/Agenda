package com.cdp.agenda.views;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

public class CircuitDiagramView extends View {
    private Paint paint;
    private Paint textPaint;
    private Paint componentPaint;
    private float scaleX, scaleY;
    private int padding = 50;

    public CircuitDiagramView(Context context) {
        super(context);
        init();
    }

    public CircuitDiagramView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setColor(Color.BLACK);
        paint.setStrokeWidth(4f);
        paint.setStyle(Paint.Style.STROKE);

        textPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        textPaint.setColor(Color.BLACK);
        textPaint.setTextSize(40f);
        textPaint.setTextAlign(Paint.Align.CENTER);

        componentPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        componentPaint.setColor(Color.BLUE);
        componentPaint.setStrokeWidth(3f);
        componentPaint.setStyle(Paint.Style.STROKE);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        
        int width = getWidth();
        int height = getHeight();
        
        scaleX = (width - 2 * padding) / 1000f;
        scaleY = (height - 2 * padding) / 800f;
        
        canvas.drawColor(Color.WHITE);
        
        // Dibujar título
        textPaint.setTextSize(50f);
        canvas.drawText("Esquema Eléctrico - Arduino Inverter", width / 2f, padding, textPaint);
        
        // Dibujar componentes
        drawArduino(canvas, 100, 200);
        drawBattery(canvas, 100, 400);
        drawMOSFET(canvas, 400, 200);
        drawMOSFET(canvas, 400, 350);
        drawMOSFET(canvas, 400, 500);
        drawMOSFET(canvas, 400, 650);
        drawTransformer(canvas, 650, 400);
        drawCapacitor(canvas, 750, 350);
        drawCapacitor(canvas, 750, 450);
        drawResistor(canvas, 250, 250);
        drawResistor(canvas, 250, 300);
        drawLoad(canvas, 850, 400);
        
        // Dibujar conexiones
        drawConnections(canvas);
        
        // Dibujar etiquetas
        drawLabels(canvas);
    }

    private void drawArduino(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        RectF arduinoRect = new RectF(scaledX, scaledY, scaledX + 120 * scaleX, scaledY + 80 * scaleY);
        paint.setColor(Color.BLUE);
        canvas.drawRect(arduinoRect, paint);
        
        textPaint.setTextSize(25f);
        textPaint.setTextAlign(Paint.Align.CENTER);
        canvas.drawText("ARDUINO", scaledX + 60 * scaleX, scaledY + 40 * scaleY, textPaint);
        
        // Pines
        float pinSpacing = 15 * scaleX;
        for (int i = 0; i < 5; i++) {
            canvas.drawLine(scaledX + 120 * scaleX, scaledY + (20 + i * 15) * scaleY,
                           scaledX + 130 * scaleX, scaledY + (20 + i * 15) * scaleY, paint);
        }
    }

    private void drawBattery(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        // Dibujar símbolo de batería
        RectF batRect = new RectF(scaledX, scaledY, scaledX + 40 * scaleX, scaledY + 60 * scaleY);
        paint.setColor(Color.RED);
        canvas.drawRect(batRect, paint);
        
        // Terminales
        canvas.drawLine(scaledX + 20 * scaleX, scaledY - 10 * scaleY, scaledX + 20 * scaleX, scaledY, paint);
        canvas.drawLine(scaledX + 20 * scaleX, scaledY + 60 * scaleY, scaledX + 20 * scaleX, scaledY + 70 * scaleY, paint);
        
        textPaint.setTextSize(20f);
        canvas.drawText("12V", scaledX + 20 * scaleX, scaledY - 20 * scaleY, textPaint);
    }

    private void drawMOSFET(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        paint.setColor(Color.BLACK);
        // Línea vertical (drain-source)
        canvas.drawLine(scaledX, scaledY, scaledX, scaledY + 60 * scaleY, paint);
        
        // Línea horizontal (gate)
        canvas.drawLine(scaledX - 30 * scaleX, scaledY + 20 * scaleY, scaledX, scaledY + 20 * scaleY, paint);
        
        // Terminales
        canvas.drawLine(scaledX, scaledY - 10 * scaleY, scaledX, scaledY, paint); // Drain
        canvas.drawLine(scaledX, scaledY + 60 * scaleY, scaledX, scaledY + 70 * scaleY, paint); // Source
        
        textPaint.setTextSize(15f);
        canvas.drawText("MOSFET", scaledX + 30 * scaleX, scaledY + 30 * scaleY, textPaint);
    }

    private void drawTransformer(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        paint.setColor(Color.BLACK);
        // Devanado primario
        for (int i = 0; i < 5; i++) {
            canvas.drawLine(scaledX, scaledY + i * 10 * scaleY, scaledX + 30 * scaleX, scaledY + i * 10 * scaleY, paint);
        }
        
        // Devanado secundario
        for (int i = 0; i < 5; i++) {
            canvas.drawLine(scaledX + 50 * scaleX, scaledY + i * 10 * scaleY, scaledX + 80 * scaleX, scaledY + i * 10 * scaleY, paint);
        }
        
        // Núcleo (línea vertical)
        canvas.drawLine(scaledX + 40 * scaleX, scaledY - 10 * scaleY, scaledX + 40 * scaleX, scaledY + 50 * scaleY, paint);
        
        textPaint.setTextSize(20f);
        canvas.drawText("TRANS", scaledX + 40 * scaleX, scaledY + 70 * scaleY, textPaint);
    }

    private void drawCapacitor(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        paint.setColor(Color.BLACK);
        // Líneas verticales paralelas
        canvas.drawLine(scaledX, scaledY, scaledX, scaledY + 30 * scaleY, paint);
        canvas.drawLine(scaledX + 20 * scaleX, scaledY, scaledX + 20 * scaleX, scaledY + 30 * scaleY, paint);
        
        // Terminales
        canvas.drawLine(scaledX, scaledY - 10 * scaleY, scaledX, scaledY, paint);
        canvas.drawLine(scaledX + 20 * scaleX, scaledY - 10 * scaleX, scaledX + 20 * scaleX, scaledY, paint);
        
        textPaint.setTextSize(15f);
        canvas.drawText("C", scaledX + 10 * scaleX, scaledY + 50 * scaleY, textPaint);
    }

    private void drawResistor(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        paint.setColor(Color.BLACK);
        // Cuerpo del resistor (rectángulo)
        RectF resistorRect = new RectF(scaledX, scaledY, scaledX + 40 * scaleX, scaledY + 15 * scaleY);
        canvas.drawRect(resistorRect, paint);
        
        // Terminales
        canvas.drawLine(scaledX - 10 * scaleX, scaledY + 7.5f * scaleY, scaledX, scaledY + 7.5f * scaleY, paint);
        canvas.drawLine(scaledX + 40 * scaleX, scaledY + 7.5f * scaleY, scaledX + 50 * scaleX, scaledY + 7.5f * scaleY, paint);
        
        textPaint.setTextSize(15f);
        canvas.drawText("R", scaledX + 20 * scaleX, scaledY - 10 * scaleY, textPaint);
    }

    private void drawLoad(Canvas canvas, float x, float y) {
        float scaledX = x * scaleX + padding;
        float scaledY = y * scaleY + padding;
        
        paint.setColor(Color.BLACK);
        // Símbolo de resistencia (zigzag)
        Path loadPath = new Path();
        loadPath.moveTo(scaledX, scaledY + 30 * scaleY);
        loadPath.lineTo(scaledX + 15 * scaleX, scaledY);
        loadPath.lineTo(scaledX + 30 * scaleX, scaledY + 30 * scaleY);
        loadPath.lineTo(scaledX + 45 * scaleX, scaledY);
        loadPath.lineTo(scaledX + 60 * scaleX, scaledY + 30 * scaleY);
        canvas.drawPath(loadPath, paint);
        
        // Terminales
        canvas.drawLine(scaledX + 30 * scaleX, scaledY - 10 * scaleY, scaledX + 30 * scaleX, scaledY, paint);
        canvas.drawLine(scaledX + 30 * scaleX, scaledY + 30 * scaleY, scaledX + 30 * scaleX, scaledY + 40 * scaleY, paint);
        
        textPaint.setTextSize(20f);
        canvas.drawText("LOAD", scaledX + 30 * scaleX, scaledY + 70 * scaleY, textPaint);
    }

    private void drawConnections(Canvas canvas) {
        paint.setColor(Color.BLACK);
        paint.setStrokeWidth(3f);
        
        // Conexiones desde Arduino a MOSFETs
        float arduinoX = 100 * scaleX + padding + 130 * scaleX;
        float arduinoY = 200 * scaleY + padding;
        
        // Gate 1
        canvas.drawLine(arduinoX, arduinoY + 20 * scaleY, 400 * scaleX + padding - 30 * scaleX, 200 * scaleY + padding + 20 * scaleY, paint);
        // Gate 2
        canvas.drawLine(arduinoX, arduinoY + 35 * scaleY, 400 * scaleX + padding - 30 * scaleX, 350 * scaleY + padding + 20 * scaleY, paint);
        // Gate 3
        canvas.drawLine(arduinoX, arduinoY + 50 * scaleY, 400 * scaleX + padding - 30 * scaleX, 500 * scaleY + padding + 20 * scaleY, paint);
        // Gate 4
        canvas.drawLine(arduinoX, arduinoY + 65 * scaleY, 400 * scaleX + padding - 30 * scaleX, 650 * scaleY + padding + 20 * scaleY, paint);
        
        // Conexiones desde batería a MOSFETs
        float batX = 100 * scaleX + padding + 20 * scaleX;
        float batY = 400 * scaleY + padding;
        
        // Línea de alimentación positiva
        canvas.drawLine(batX, batY - 10 * scaleY, batX + 250 * scaleX, batY - 10 * scaleY, paint);
        canvas.drawLine(batX + 250 * scaleX, batY - 10 * scaleY, batX + 250 * scaleX, 200 * scaleY + padding - 10 * scaleY, paint);
        
        // Línea de tierra
        canvas.drawLine(batX, batY + 70 * scaleY, batX + 250 * scaleX, batY + 70 * scaleY, paint);
        canvas.drawLine(batX + 250 * scaleX, batY + 70 * scaleY, batX + 250 * scaleX, 650 * scaleY + padding + 70 * scaleY, paint);
        
        // Conexiones desde MOSFETs al transformador
        float mosfetX = 400 * scaleX + padding;
        canvas.drawLine(mosfetX, 200 * scaleY + padding - 10 * scaleY, 650 * scaleX + padding, 400 * scaleY + padding, paint);
        canvas.drawLine(mosfetX, 650 * scaleY + padding + 70 * scaleY, 650 * scaleX + padding, 400 * scaleY + padding + 50 * scaleY, paint);
        
        // Conexiones desde transformador a carga
        float transX = 650 * scaleX + padding + 80 * scaleX;
        canvas.drawLine(transX, 400 * scaleY + padding, 850 * scaleX + padding + 30 * scaleX, 400 * scaleY + padding, paint);
        canvas.drawLine(transX, 400 * scaleY + padding + 50 * scaleY, 850 * scaleX + padding + 30 * scaleX, 400 * scaleY + padding + 30 * scaleY, paint);
        
        // Conexiones de condensadores
        canvas.drawLine(transX, 400 * scaleY + padding, 750 * scaleX + padding + 10 * scaleX, 350 * scaleY + padding, paint);
        canvas.drawLine(transX, 400 * scaleY + padding + 50 * scaleY, 750 * scaleX + padding + 10 * scaleX, 450 * scaleY + padding + 30 * scaleY, paint);
    }

    private void drawLabels(Canvas canvas) {
        textPaint.setTextSize(18f);
        textPaint.setColor(Color.BLACK);
        
        // Etiquetas de conexiones
        canvas.drawText("G1", 300 * scaleX + padding, 200 * scaleY + padding + 20 * scaleY, textPaint);
        canvas.drawText("G2", 300 * scaleX + padding, 350 * scaleY + padding + 20 * scaleY, textPaint);
        canvas.drawText("G3", 300 * scaleX + padding, 500 * scaleY + padding + 20 * scaleY, textPaint);
        canvas.drawText("G4", 300 * scaleX + padding, 650 * scaleY + padding + 20 * scaleY, textPaint);
        
        // Etiquetas de voltajes
        textPaint.setTextSize(16f);
        canvas.drawText("V+", 200 * scaleX + padding, 380 * scaleY + padding, textPaint);
        canvas.drawText("GND", 200 * scaleX + padding, 450 * scaleY + padding, textPaint);
        
        // Nota informativa
        textPaint.setTextSize(30f);
        textPaint.setColor(Color.BLUE);
        canvas.drawText("220V AC", 850 * scaleX + padding + 30 * scaleX, 350 * scaleY + padding, textPaint);
    }
}
