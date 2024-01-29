import PropTypes from 'prop-types';
import { useState, useEffect } from 'react';
import celsius from '../assets/celsius.svg';
import '../customStyles.css';

const Gauge = ({ temperatura }) => {
	const [arcDashArray, setArcDashArray] = useState('0 282.78');

	const calculateArcDashArray = (value) => {
		const arcLength = 282.78; // Valor original del atributo stroke-dasharray
		const step = arcLength / (30 - 10);
		const dashArrayValue = (value - 10) * step;
		return `${dashArrayValue} ${arcLength - dashArrayValue}`;
	};

	useEffect(() => {
		setArcDashArray(calculateArcDashArray(temperatura));
	}, [temperatura]);

	return (
		<div id='gauge'>
			<div id='major-ticks'>
				<span>10℃</span>
				<span>20℃</span>
				<span>30℃</span>
			</div>
			<div id='minor-ticks'>
				{[...Array(20)].map((_, i) => (
					<span key={i} title={`--i:${i + 1}`}></span>
				))}
			</div>
			<div id='minor-ticks-bottom-mask'></div>
			<div id='bottom-circle'></div>
			<svg
				version='1.1'
				baseProfile='full'
				width='190'
				height='190'
				xmlns='http://www.w3.org/2000/svg'
			>
				<linearGradient id='gradient' x1='0' x2='1' y1='0' y2='0'>
					<stop offset='0%' stopColor='#b96e85' />
					<stop offset='100%' stopColor='#ae69bb' />
				</linearGradient>
				<path
					id='arcPath'
					d='M5 95 A80 80 0 0 1 185 95'
					stroke='url(#gradient)'
					fill='none'
					strokeWidth='10'
					strokeLinecap='round'
					strokeDasharray={arcDashArray}
				/>
			</svg>
			<div id='center-circle'>
				<span id='name'>Temperatura</span>
				<span id='temperature'>{temperatura}</span>
				<img src={celsius} alt='' />
			</div>
		</div>
	);
};

Gauge.propTypes = {
	temperatura: PropTypes.number.isRequired,
};

export default Gauge;
