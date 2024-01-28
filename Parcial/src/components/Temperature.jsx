import PropTypes from 'prop-types';
import temperature from '../assets/temperature.svg';

const Temperature = ({ temperatura }) => {
	return (
		<div className='h-52 w-52 shadow-xl rounded-2xl flex flex-col gap-5 justify-center items-center'>
			<img src={temperature} alt='' className='h-20 w-20' />
			<p className='text-4xl'>{temperatura}°C</p>
		</div>
	);
};

Temperature.propTypes = {
	temperatura: PropTypes.number.isRequired,
};

export default Temperature;
