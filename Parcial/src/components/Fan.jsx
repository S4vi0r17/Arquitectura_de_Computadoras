import fan from '../assets/fan.svg';

const Fan = () => {
	return (
		<div className='h-52 w-52 shadow-xl rounded-2xl flex flex-col gap-5 justify-center items-center col-start-2 row-start-2'>
			<img src={fan} alt='' className='h-20 w-20 animate-spin' />
			<p className='text-4xl'>Encendido</p>
		</div>
	);
};

export default Fan;
