import express from 'express';
import path from 'path';
import cors from 'cors';
import helmet from 'helmet';
import morgan from 'morgan';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

const app = express();
const PORT = process.env.PORT || 4000;

let ultimosDatos = {
  temperatura: 0,
  humedad: 0,
  ventiladorEncendido: false,
};

app.use(cors());
app.use(helmet());
app.use(morgan('dev'));

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.post('/datos', (req, res) => {
  const { temperatura, humedad, ventiladorEncendido } = req.body;
  console.log(`Temperatura: ${temperatura}°C, Humedad: ${humedad}%, Ventilador: ${ventiladorEncendido ? 'Encendido' : 'Apagado'}`);

  ultimosDatos = {
    temperatura,
    humedad,
    ventiladorEncendido,
  };

  res.status(200).send('Datos recibidos correctamente');
});

app.get('/api/datos', (req, res) => {
  res.json(ultimosDatos);
});

app.listen(PORT, () => {
  console.log(`Servidor escuchando en el puerto http://localhost:${PORT}/datos`);
});
