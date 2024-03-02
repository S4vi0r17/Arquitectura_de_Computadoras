import PropTypes from 'prop-types';
import fan from '../assets/fan.svg';

const Fan = ({ ventiladorEncendido }) => {
	return (
		<div className='h-auto w-full shadow-xl rounded-2xl flex flex-col gap-5 justify-center items-center md:col-span-2 lg:col-start-5 lg:col-span-1 row-start-1'>
			<img
				src={fan}
				alt=''
				className={`h-20 w-20 ${
					ventiladorEncendido ? 'animate-spin' : ''
				}`}
			/>
			<p className='text-3xl'>
				{ventiladorEncendido ? 'Encendido' : 'Apagado'}
			</p>
		</div>
	);
};

Fan.propTypes = {
	ventiladorEncendido: PropTypes.bool.isRequired,
};

export default Fan;
